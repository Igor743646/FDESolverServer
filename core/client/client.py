import pathlib
import argparse
from fastapi import FastAPI
from fastapi.responses import FileResponse, RedirectResponse
from fastapi.staticfiles import StaticFiles
from src.router import router as home_router

CLIENT_PROGRAM_PATH = pathlib.Path(__file__).parent.resolve()
DEPENDENCIES_FILES_PATH = CLIENT_PROGRAM_PATH / "static"

app = FastAPI(
    title="FDESolver",
    version="0.1.0",
    debug=True
)
app.include_router(home_router)
app.mount("/static", StaticFiles(directory=DEPENDENCIES_FILES_PATH, html=True), name="static")


@app.get("/", include_in_schema=False)
async def homeRedirect():
    return RedirectResponse(url="/home")


def parse():
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", 
                        default="localhost", 
                        help="Host for uvicorn")
    return parser.parse_args()


if __name__ == "__main__":
    import uvicorn

    args = parse()

    uvicorn.run("client:app", host=args.host, port=8002, reload=True,
                reload_dirs=[CLIENT_PROGRAM_PATH],
                reload_includes=["*.css", "*.js", "*.html"])
