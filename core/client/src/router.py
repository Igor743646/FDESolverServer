import os
import pathlib

import json
import grpc
from google.protobuf import json_format
from urllib.parse import unquote
from fastapi import APIRouter, Form, Request
from fastapi.templating import Jinja2Templates

from libs import result_drawer as ResultDrawer
from libs.logger import log
import config_pb2
from server_pb2_grpc import TFDESolverServerStub

MODULE_PATH = pathlib.Path(__file__).parent.resolve()
DEPENDENCIES_FILES_PATH = MODULE_PATH / "../static"
TEMPLATES_FILES_PATH = DEPENDENCIES_FILES_PATH / "templates"
FAVICON_ICO_PATH = MODULE_PATH / "../favicon.ico"

router = APIRouter(
    prefix="/home",
    tags=["Home"]
)

FDESolverServerHost = os.getenv("SERVER_HOST", "localhost")
print(f"FDESolverServerHost : {FDESolverServerHost}")
FDESolverServerChannel = grpc.insecure_channel(
    f"{FDESolverServerHost}:50051"
)
FDESolverServerClient = TFDESolverServerStub(FDESolverServerChannel)

templates = Jinja2Templates(directory=TEMPLATES_FILES_PATH)

DEFAULT_CONFIG = '''{
    "LeftBound": "-0.4",
    "RightBound": "0.4",   
    "MaxTime": "0.2",                 
    "Alpha": "1.8",
    "Gamma": "0.9",            
    "SpaceStep": "0.02",
    "TimeStep": "0.001",     
    "Beta": "0.5",  
    "DiffusionCoefficient": "0.001",     
    "DemolitionCoefficient": "0.0",    
    "ZeroTimeState": "((-0.01 < x) && (x < 0.01)) ? 50.0 : 0.0",            
    "SourceFunction": "0.0",   
    "LeftBoundState": "0.0",           
    "RightBoundState": "0.00000",          
    "BordersAvailable": false,          
    "StochasticIterationCount": "1000",
    "RealSolutionName": "$u(x, t) = ??",                        
    "RealSolution" : "0.0",
    "SolveMethods": "MGL;MRL;SGL;SRL"
}'''

@router.get("")
async def home(request: Request):
    return templates.TemplateResponse("index.html", {"request": request, "config" : DEFAULT_CONFIG})

@router.post("")
async def runTask(request: Request, config: str = Form(...)):
    log.debug(config)

    result = {
        "request": request,
        "config": config,
    }

    try:
        json_object = dict(json.loads(unquote(config)))
    except json.JSONDecodeError as exception:
        log.warning(f"Can not parse JSON: {exception.msg}")
        result["error"] = exception.msg
        return templates.TemplateResponse("index.html", result)
    except BaseException as undefined_error:
        log.error(f"Unknown error: {undefined_error.msg}")
        result["error"] = undefined_error.msg
        return templates.TemplateResponse("index.html", result)

    proto_request = json_format.ParseDict(json_object, config_pb2.TClientConfig(), ignore_unknown_fields=True)
    log.debug(proto_request)
    
    try:
        response, call = FDESolverServerClient.RunTask.with_call(proto_request)
        dcall = dict(call.trailing_metadata())
        if "work-time" in dcall:
            result["work_time"] = dcall["work-time"]
        rdResult = ResultDrawer.Results(response)
        result["images"] = []
        ResultDrawer.draw_surface(rdResult.config, rdResult)
        result["images"].append(ResultDrawer.ReturnBase64Image())
    except grpc.RpcError as exception:
        log.error(f"Rpc server not working: {exception}")
        result["error"] = str(exception)
        return templates.TemplateResponse("index.html", result)
    except BaseException as undefined_error:
        log.error(f"Unknown error: {undefined_error}")
        result["error"] = str(undefined_error)
        return templates.TemplateResponse("index.html", result)

    return templates.TemplateResponse("index.html", result)
