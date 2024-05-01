import sys

sys.path.append(r"C:/Users/Igor/Desktop/FDESolverServer/core/protos/py/")
sys.path.append(r"C:/Users/Igor/Desktop/FDESolverServer/core/server/fde_solver/libs/python")

import pathlib
import json
import base64
import logging
from urllib.parse import unquote
from fastapi import FastAPI, Request, HTTPException
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from fastapi.responses import FileResponse

import grpc
from google.protobuf import json_format
from server_pb2_grpc import TFDESolverServerStub
import config_pb2

import io
import matplotlib
matplotlib.use('AGG')
import matplotlib.pyplot as plt
import numpy as np
# import result_drawer

CLIENT_PROGRAM_PATH = pathlib.Path(__file__).parent.resolve()
DEPENDENCIES_FILES_PATH = CLIENT_PROGRAM_PATH / "static"

app = FastAPI(
    title="FDESolver"
)
app.mount("/static", StaticFiles(directory=DEPENDENCIES_FILES_PATH, html=True), name="static")

templates = Jinja2Templates(directory=DEPENDENCIES_FILES_PATH / "templates")

class CustomFormatter(logging.Formatter):

    grey = "\x1b[38;20m"
    yellow = "\x1b[33;20m"
    green = "\x1b[32;20m"
    blue = "\x1b[34;20m"
    red = "\x1b[31;20m"
    bold_red = "\x1b[31;1m"
    reset = "\x1b[0m"
    time = "[%(asctime)s] "
    header = " %(levelname)s: "
    message = "%(message)s"
    bottom = " (%(filename)s:%(lineno)d)"

    FORMATS = {
        logging.DEBUG: time + blue + header + reset + message + blue + bottom + reset,
        logging.INFO: time + green + header + reset  + message + green + bottom + reset,
        logging.WARNING: time + yellow + header + reset  + message + yellow + bottom + reset,
        logging.ERROR: time + red + header + reset  + message + red + bottom + reset,
        logging.CRITICAL: time + bold_red + header + reset  + message + bold_red + bottom + reset
    }

    def format(self, record):
        log_fmt = self.FORMATS.get(record.levelno)
        formatter = logging.Formatter(log_fmt)
        return formatter.format(record)

log = logging.getLogger("FDESolverServerLogger")
log.handlers.clear()
log.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
ch.setFormatter(CustomFormatter())
log.addHandler(ch)

class Result(object):

    def __init__(self, proto_res) -> None:
        self.proto_file = proto_res
        self.method_name : str = proto_res.MethodName
        self.config = proto_res.Config
        self.solver_matrix : np.ndarray | None = None

        self.field = np.array(proto_res.Field.Data)
        self.field.resize((proto_res.Field.Rows, proto_res.Field.Columns))

        if proto_res.HasField("SolveMatrix"):
            self.solver_matrix = np.array(proto_res.SolveMatrix.Data)
            self.solver_matrix.resize((proto_res.SolveMatrix.Rows, proto_res.SolveMatrix.Columns))


class Results(object):

    def __init__(self, results):
        self.results : list[Result] = []
        self.real_solution : np.ndarray | None = None
        self.real_solution_name : str | None = None 

        self.config = results.Task
        for result in results.Results:
            self.results.append(Result(result))

        if results.HasField("RealSolution"):
            self.real_solution = np.array(results.RealSolution.Data)
            self.real_solution.resize((results.RealSolution.Rows, results.RealSolution.Columns))
            self.real_solution_name = results.RealSolutionName

def draw_surface(config, results):
    tasks = results.results

    h, w = 1, len(tasks)

    if results.real_solution is not None:
        w += 1

    _, ax = plt.subplots(h, w, figsize=(10*w, 7), subplot_kw={"projection": "3d"})

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)
    Y = np.linspace(0, config.MaxTime, config.TimeCount + 1)
    X, Y = np.meshgrid(X, Y)

    for i, task in enumerate(tasks):
        ax[i].plot_surface(X, Y, task.field)
        ax[i].set_title(task.method_name)
        ax[i].set(xlabel='x', ylabel="y", zlabel="u(x, t)")
        ax[i].zaxis.set_rotate_label(False)

    if results.real_solution is not None:
        ax[-1].plot_surface(X, Y, results.real_solution)
        ax[-1].set_title(results.real_solution_name)
        ax[-1].set(xlabel='x', ylabel="y", zlabel="u(x, t)")
        ax[-1].zaxis.set_rotate_label(False)

    bio = io.BytesIO()
    plt.savefig(bio, format="png")
    bio.seek(0)
    return base64.encodebytes(bio.getvalue()).decode()

@app.get("/")
async def home(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})

@app.get('/favicon.ico', include_in_schema=False)
async def favicon():
    return FileResponse(CLIENT_PROGRAM_PATH / "favicon.ico")

@app.get("/{b64_json_config}")
async def home(request: Request, b64_json_config: str):
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
            response = stub.RunTask(proto_request)
            rdResult = Results(response)
            log.debug(rdResult.results[0].field[50])
            result["images"] = []
            
            result["images"].append(draw_surface(rdResult.config, rdResult))
        except grpc.RpcError as exception:
            log.warning(f"Rpc server not working: {exception}")
            result["error"] = str(exception)
            return templates.TemplateResponse("index.html", result)

    return templates.TemplateResponse("index.html", result)

if __name__ == "__main__":
    import uvicorn

    uvicorn.run("main:app", host="localhost", port=8002, reload=True, 
                reload_dirs=[DEPENDENCIES_FILES_PATH],
                reload_includes=["*.css", "*.js", "*.html"])
