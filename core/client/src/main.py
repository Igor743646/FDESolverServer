import sys

sys.path.append(r"C:/Users/Igor/Desktop/FDESolverServer/core/protos/py/")

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

@app.get("/")
async def home(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})

@app.get('/favicon.ico', include_in_schema=False)
async def favicon():
    return FileResponse(CLIENT_PROGRAM_PATH / "favicon.ico")

@app.get("/{b64_json_config}")
async def home(request: Request, b64_json_config: str):
    log.debug(b64_json_config)
    json_config = unquote(base64.b64decode(b64_json_config))

    try:
        json_object = dict(json.loads(json_config))
    except json.JSONDecodeError as exception:
        log.warning(f"Can not parse JSON: {exception.msg}")
        return templates.TemplateResponse("index.html", {"request": request, "error": exception.msg})
    
    json_object["BordersAvailable"] = (json_object["BordersAvailable"] in ["True", "true"])

    proto_request = json_format.ParseDict(json_object, config_pb2.TClientConfig(), ignore_unknown_fields=True)
    log.debug(proto_request)
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = TFDESolverServerStub(channel)
        response = stub.RunTask(proto_request)
        print(response.RealSolutionName)

    return templates.TemplateResponse("index.html", {"request": request})

if __name__ == "__main__":
    import uvicorn

    uvicorn.run("main:app", host="localhost", port=8002, reload=True, 
                reload_dirs=[DEPENDENCIES_FILES_PATH],
                reload_includes=["*.css", "*.js"])
