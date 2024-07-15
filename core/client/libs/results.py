import sys, pathlib
import numpy as np

PROTOBUFS_DIR = str(pathlib.Path(__file__) / "../generated/")
sys.path.append(PROTOBUFS_DIR)

from .generated import result_pb2
from .generated import config_pb2

class Result(object):

    def __init__(self, proto_res : result_pb2.TResult) -> None:
        self.method_name : str = proto_res.MethodName
        self.field : np.ndarray = np.array(proto_res.Field.Data)
        self.field.resize((proto_res.Field.Rows, proto_res.Field.Columns))


class Results(object):

    def __init__(self, results : result_pb2.TResults):
        self.results : list[Result] = []
        self.real_solution : np.ndarray | None = None
        self.real_solution_name : str | None = None 
        self.config : config_pb2.TSolverConfig = results.Task

        for result in results.Results:
            self.results.append(Result(result))

        if results.HasField("RealSolution"):
            self.real_solution = np.array(results.RealSolution.Data)
            self.real_solution.resize((results.RealSolution.Rows, results.RealSolution.Columns))
            self.real_solution_name = results.RealSolutionName


    def HasRealSolution(self) -> bool:
        return self.real_solution is not None