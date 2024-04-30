import sys
sys.path.append(r"C:/Users/Igor/Desktop/FDESolverServer/core/protos/py/")

import pathlib
from fastapi import FastAPI, Request
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates

import grpc
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
    # print(json_config)
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = TFDESolverServerStub(channel)
        response = stub.RunTask(config_pb2.TClientConfig())
        print(response.RealSolutionName)

    return templates.TemplateResponse("index.html", {"request": request})

if __name__ == "__main__":
    import uvicorn

    uvicorn.run("main:app", host="localhost", port=8002, reload=True, 
                reload_dirs=[DEPENDENCIES_FILES_PATH],
                reload_includes=["*.css", "*.js"])
