import os
import pathlib
import traceback
import json
import grpc
from google.protobuf import json_format
from urllib.parse import unquote
from fastapi import APIRouter, Form, Request
from fastapi.templating import Jinja2Templates

from libs import result_drawer as ResultDrawer
from libs.logger import log
import config_pb2
import result_pb2
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
    "LeftBound": "0.0",
    "RightBound": "1.0",   
    "MaxTime": "1.0",                 
    "Alpha": "1.5",
    "Gamma": "0.9",            
    "SpaceStep": "0.052",
    "TimeStep": "0.01",     
    "Beta": "1.0",  
    "DiffusionCoefficient": "0.0001 * Gamma(3.0 - 1.5) / Gamma(3.0) * pow(x, 1.5)",     
    "DemolitionCoefficient": "0.0",    
    "ZeroTimeState": "0.0",            
    "SourceFunction": "Gamma(3.0) / Gamma(3.0 - 0.9) * (pow(t, 2.0 - 0.9) * pow(x, 2.0)) - 0.0001 * pow(x, 2.0) * pow(t, 2.0)",   
    "LeftBoundState": "0.0",           
    "RightBoundState": "pow(t, 2.0)",          
    "BordersAvailable": true,          
    "StochasticIterationCount": "80",
    "RealSolutionName": "$u(x, t) = x^2 \\\\cdot t^2$",                        
    "RealSolution" : "pow(x, 2.0) * pow(t, 2.0)",
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
        drawer = ResultDrawer.ResultDrawer(response)
        result["images"] = drawer.DrawResults()
        # rdResults = ResultDrawer.Results(response)
        # result["images"] = []
        # ResultDrawer.draw_flat_field(rdResults.config, rdResults.results)
        # result["images"].append(ResultDrawer.ReturnBase64Image())
        # ResultDrawer.draw_surface(rdResults.config, rdResults)
        # result["images"].append(ResultDrawer.ReturnBase64Image())
        # ResultDrawer.draw_error(rdResults.config, rdResults)
        # result["images"].append(ResultDrawer.ReturnBase64Image())
    except grpc.RpcError as exception:
        log.error(f"Rpc server not working: {exception}")
        result["error"] = str(exception) + f" Stack: {traceback.format_exc()}"
        return templates.TemplateResponse("index.html", result)
    except BaseException as undefined_error:
        log.error(f"Unknown error: {undefined_error}")
        result["error"] = str(undefined_error) + f" Stack: {traceback.format_exc()}"
        return templates.TemplateResponse("index.html", result)

    return templates.TemplateResponse("index.html", result)
