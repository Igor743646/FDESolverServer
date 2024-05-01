import sys
sys.path.append(r"C:/Users/Igor/Desktop/FDESolverServer/core/protos/py/")

import pathlib
import json
import base64
from fastapi import FastAPI, Request
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates

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

@app.get("/")
async def home(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})

@app.get("/{json_config}")
async def home(request: Request, json_config: str):
    print("Decoded json: ", base64.b64decode(json_config))

    proto_request = config_pb2.TClientConfig()
    json_object = dict(json.loads(base64.b64decode(json_config)))
    print(json_object)
    json_object["BordersAvailable"] = (json_object["BordersAvailable"] in ["True", "true"])
    proto_request = json_format.ParseDict(json_object, proto_request, ignore_unknown_fields=True)
    print(proto_request)
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
