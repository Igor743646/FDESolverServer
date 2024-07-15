import base64
import io
import matplotlib
import argparse
from argparse import ArgumentParser
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

from .results import result_pb2
from .results import Result, Results

matplotlib.use("Agg")
matplotlib.rcParams["savefig.format"] = "png"
matplotlib.rcParams["savefig.dpi"] = 'figure'
matplotlib.rcParams["savefig.bbox"] = 'tight'

class ResultDrawer:

    def __init__(self, results : result_pb2.TResults):
        self.results = Results(results)
        self.config = self.results.config


    def ResetSettings(self):
        matplotlib.rcParams["figure.titlesize"] = "xx-large"
        matplotlib.rcParams['axes.titlesize'] = "large"


    def RealSolutionSettings(self):
        matplotlib.rcParams["figure.titlesize"] = 28
        matplotlib.rcParams['axes.titlesize'] = 22


    def ReturnBase64Image(self) -> bytes:
        bio = io.BytesIO()
        plt.savefig(bio, bbox_inches="tight", pad_inches=0.3)
        plt.close()
        return base64.encodebytes(bio.getvalue()).decode()
    

    def DrawResults(self) -> list[bytes]:
        self.ResetSettings()
        images = []

        for result in self.results.results:
            images.append(self.DrawResult(result))
        
        return images


    def DrawResult(self, result : Result) -> bytes:
        if self.results.HasRealSolution():
            self.RealSolutionSettings()
            figure = plt.figure(figsize=(10 * 3, 18), layout="compressed")
            subfigures = figure.subfigures(2, 1)
            subfigures1 = subfigures[0].subfigures(1, 2) # first row
            subfigures2 = subfigures[1].subfigures(1, 3) # second row
        else:
            figure = plt.figure(figsize=(10 * 2, 8), layout="compressed")
            subfigures1 = figure.subfigures(1, 2)

        figure.suptitle(result.method_name)
        self.DrawHeatMap(subfigures1[0], result)
        self.DrawSurface(subfigures1[1], result)

        if self.results.HasRealSolution():
            self.DrawRealSolution(subfigures2[0])
            self.DrawErrorSurface(subfigures2[1], result)
            self.DrawErrorMean(subfigures2[2], result)

        return self.ReturnBase64Image()


    def DrawHeatMap(self, subfigure : matplotlib.figure.SubFigure, result : Result):
        ax = subfigure.subplots(1, 1)

        im = ax.imshow(result.field, origin="lower", aspect='equal',
                                  extent=(self.config.LeftBound, self.config.RightBound, 0, self.config.MaxTime))
        ax.set(xlabel='x', ylabel="t")
        ax.set_title("Heat Map")

        subfigure.colorbar(im, ax=ax)


    def DrawSurface(self, subfigure : matplotlib.figure.SubFigure, result : Result):
        ax = subfigure.subplots(1, 1, subplot_kw={"projection": "3d"})

        X = np.linspace(self.config.LeftBound, self.config.RightBound, self.config.SpaceCount + 1)
        Y = np.linspace(0, self.config.MaxTime, self.config.TimeCount + 1)
        X, Y = np.meshgrid(X, Y)

        ax.plot_surface(X, Y, result.field)
        ax.set(xlabel='x', ylabel="t", zlabel="u(x, t)")
        ax.set_title("FDESolver Solution")


    def DrawRealSolution(self, subfigure : matplotlib.figure.SubFigure):
        ax = subfigure.subplots(1, 1, subplot_kw={"projection": "3d"})

        X = np.linspace(self.config.LeftBound, self.config.RightBound, self.config.SpaceCount + 1)
        Y = np.linspace(0, self.config.MaxTime, self.config.TimeCount + 1)
        Xm, Ym = np.meshgrid(X, Y)

        ax.plot_surface(Xm, Ym, self.results.real_solution)
        ax.set_title(self.results.real_solution_name)
        ax.set(xlabel='x', ylabel="t", zlabel="u(x, t)")


    def DrawErrorSurface(self, subfigure : matplotlib.figure.SubFigure, result : Result):
        ax = subfigure.subplots(1, 1, subplot_kw={"projection": "3d"})

        X = np.linspace(self.config.LeftBound, self.config.RightBound, self.config.SpaceCount + 1)
        Y = np.linspace(0, self.config.MaxTime, self.config.TimeCount + 1)
        Xm, Ym = np.meshgrid(X, Y)

        error = np.abs(result.field - self.results.real_solution)
        ax.plot_surface(Xm, Ym, error)
        ax.set_title("Error")
        ax.set(xlabel='x', ylabel="t", zlabel="u(x, t)")


    def DrawErrorMean(self, subfigure : matplotlib.figure.SubFigure, result : Result):
        ax = subfigure.subplots(1, 1)

        Y = np.linspace(0, self.config.MaxTime, self.config.TimeCount + 1)

        error = np.abs(result.field - self.results.real_solution)
        ax.boxplot(error.T, showmeans=True, showfliers=False)
        ax.set_xticks(ax.get_xticks()[::10], Y[::10])
        ax.set_title("Mean by x error changing")


def parse_args() -> argparse.Namespace:
    parser = ArgumentParser()

    parser.add_argument("-f", "--file", 
                        required=True, help="file path")
    parser.add_argument("-o", "--out", default="./",
                        required=False, help="path for saving images")
    parser.add_argument("-ts", "--time-slice", default=None, type=int,
                        required=False, help="Time slice of solution")
    parser.add_argument("-v", "--verbose", action='store_false', 
                        help="Show images or not")

    return parser.parse_args()


def draw_time_slice(config, time_slice : int, results : Results):
    tasks = results.results

    _, ax = plt.subplots(1, 1, figsize=(10, 7))

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)

    if results.real_solution is not None:
        Y_real_solution = results.real_solution[time_slice]
        ax.plot(X, Y_real_solution, label=f"{results.real_solution_name}", linewidth=1.5)

    for i, task in enumerate(tasks):
        Y_solution = task.field[time_slice]
        ax.plot(X, Y_solution, label=f"{task.method_name}", linewidth=2.0)

    ax.legend(loc='best')
    ax.grid(True)
    ax.set_title(f"Time slice t={time_slice * config.TimeStep}")
    ax.set(xlabel='x', ylabel=f"u(x, {time_slice * config.TimeStep})")


def draw_slices_gif(config, results : Results):
    tasks = results.results

    count = len(tasks) + (1 if results.real_solution is not None else 0)

    ymin, ymax = np.min([np.min(i.field) for i in tasks]), np.max([np.max(i.field) for i in tasks])
    fig, ax = plt.subplots(1, 1, figsize=(15, 10))
    lines = ax.plot(*[[] for i in range(2 * count)])

    def init():
        ax.set_xlim((config.LeftBound, config.RightBound))
        ax.set_ylim((ymin, ymax))
        
        for line in lines:
            line.set_data([], [])
        return lines

    X = np.linspace(config.LeftBound, config.RightBound, config.SpaceCount + 1)

    def animate(i):
        for line_id, line in enumerate(lines):
            if results.real_solution is not None and line_id == len(lines) - 1:
                Y = results.real_solution
                label = results.real_solution_name
            else:
                Y = tasks[line_id].field
                label = tasks[line_id].method_name
            
            line.set_data(X, Y[i])
            line.set_label(f"{label}")
        ymin, ymax = ax.get_ylim()
        ax.set_ylim((ymin, np.max([np.max(task.field[i]) for task in tasks] + [ymin + 0.1])))
        ax.set_title(f"Time slice t={i * config.TimeStep:.2f}")
        ax.grid(True)
        ax.legend(loc='best')
        return lines
    
    anim = animation.FuncAnimation(fig, func=animate, init_func=init,
                                   frames=config.TimeCount + 1, interval=20)
    
    return anim


def draw(results : Results, arguments : argparse.Namespace):
    outputs = {
        "TS" : arguments.out + 'Time Slice',
        "DS" : arguments.out + 'Dynamic time slices',
    }

    if arguments.time_slice is not None:
        draw_time_slice(results.config, 
                        np.clip(arguments.time_slice, 0, results.config.TimeCount), 
                        results)
        plt.savefig(outputs['TS'])
    
    gif = draw_slices_gif(results.config, results)
    FFwriter = animation.FFMpegWriter(fps=60)
    gif.save(outputs["DS"] + ".mp4", writer = FFwriter)
    gif.save(outputs["DS"] + ".gif", fps=60)


def main(arguments : argparse.Namespace):
    res = result_pb2.TResults()
    
    with open(arguments.file, mode="rb") as file:
        res.ParseFromString(file.read())

    results = Results(res)
    draw(results, arguments)
    plt.close()


if __name__ == "__main__":
    args = parse_args()

    main(args)