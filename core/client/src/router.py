import sys
import pathlib
sys.path.append(str(pathlib.Path(__file__) / "../../../protos/py/"))

import base64
import json
import logging
import grpc
from google.protobuf import json_format
from urllib.parse import unquote
from fastapi import APIRouter, Request
from fastapi.templating import Jinja2Templates

import config_pb2
from server_pb2_grpc import TFDESolverServerStub
import libs.result_drawer as ResultDrawer

MODULE_PATH = pathlib.Path(__file__).parent.resolve()
DEPENDENCIES_FILES_PATH = MODULE_PATH / "../static"
TEMPLATES_FILES_PATH = DEPENDENCIES_FILES_PATH / "templates"
FAVICON_ICO_PATH = MODULE_PATH / "../favicon.ico"

router = APIRouter(
    prefix="/home",
    tags=["Home"]
)

templates = Jinja2Templates(directory=TEMPLATES_FILES_PATH)

log = logging.getLogger("FDESolverServerLogger")

@router.get("")
async def home(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})


@router.get("/{b64_json_config}")
async def runTask(request: Request, b64_json_config: str):
    log.debug(b64_json_config)

    result = {
        "request": request
    }

    try:
        json_config = unquote(base64.b64decode(b64_json_config))
        json_object = dict(json.loads(json_config))
    except json.JSONDecodeError as exception:
        log.warning(f"Can not parse JSON: {exception.msg}")
        result["error"] = exception.msg
        return templates.TemplateResponse("index.html", result)
    except BaseException as undefined_error:
        log.error(undefined_error)
        raise undefined_error

    json_object["BordersAvailable"] = True if (json_object["BordersAvailable"] in ["True", "true"]) else False

    proto_request = json_format.ParseDict(json_object, config_pb2.TClientConfig(), ignore_unknown_fields=True)
    log.debug(proto_request)
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = TFDESolverServerStub(channel)
        try:
            response, call = stub.RunTask.with_call(proto_request)
            dcall = dict(call.trailing_metadata())
            if "work-time" in dcall:
                log.debug(dcall["work-time"])
                result["work_time"] = dcall["work-time"]
            rdResult = ResultDrawer.Results(response)
            result["images"] = []
            ResultDrawer.draw_surface(rdResult.config, rdResult)
            result["images"].append(ResultDrawer.ReturnBase64Image())
        except grpc.RpcError as exception:
            log.warning(f"Rpc server not working: {exception}")
            result["error"] = str(exception)
            return templates.TemplateResponse("index.html", result)

    return templates.TemplateResponse("index.html", result)
