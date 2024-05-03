import logging
import sys

sys.path.append(r"C:/Users/Igor/Desktop/FDESolverServer/core/protos/py/")
sys.path.append(r"C:/Users/Igor/Desktop/FDESolverServer/core/server/fde_solver/libs/python")

import pathlib
import json
import base64

from urllib.parse import unquote
from fastapi import FastAPI, Request, HTTPException
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from fastapi.responses import FileResponse

import grpc
from google.protobuf import json_format
from server_pb2_grpc import TFDESolverServerStub
import config_pb2

import matplotlib
matplotlib.use("Agg")
import numpy as np
import libs.result_drawer as ResultDrawer
import libs.logger

CLIENT_PROGRAM_PATH = pathlib.Path(__file__).parent.resolve()
DEPENDENCIES_FILES_PATH = CLIENT_PROGRAM_PATH / "static"

app = FastAPI(
    title="FDESolver",
    version="0.1.0",
    debug=True
)

app.mount("/static", StaticFiles(directory=DEPENDENCIES_FILES_PATH, html=True), name="static")

templates = Jinja2Templates(directory=DEPENDENCIES_FILES_PATH / "templates")

log = logging.getLogger("FDESolverServerLogger")

@app.get("/")
async def home(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})

@app.get('/favicon.ico', include_in_schema=False)
async def favicon():
    return FileResponse(CLIENT_PROGRAM_PATH / "favicon.ico")

@app.get("/{b64_json_config}")
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

if __name__ == "__main__":
    import uvicorn

    uvicorn.run("client:app", host="localhost", port=8002, reload=True, 
                reload_dirs=[DEPENDENCIES_FILES_PATH],
                reload_includes=["*.css", "*.js", "*.html"])
