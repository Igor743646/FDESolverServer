# FDESolverServer
[![Conan Center](https://img.shields.io/conan/v/catch2?label=catch2&color=orange)](https://github.com/catchorg/Catch2)
[![Conan Center](https://img.shields.io/conan/v/boost?label=boost&color=%23aaf0ff)](https://github.com/boostorg/boost)
[![Conan Center](https://img.shields.io/conan/v/protobuf?label=protobuf)](https://github.com/protocolbuffers/protobuf)
[![Conan Center](https://img.shields.io/conan/v/grpc?label=grpc)](https://github.com/grpc/grpc)
[![Conan Center](https://img.shields.io/conan/v/antlr4-cppruntime?label=antlr4-cppruntime&color=red)](https://github.com/antlr/antlr4)

FDESolverServer - is an open-source service for calculating fractional differential equations.
It is based on the GRPC messaging protocol and is presented in two variations: a client-server application and a console utility.

## How to build it?

The project uses the Cmake build system with the Conan package manager for C++ backend.
In addition, the client part is a Go application.

Therefore, these things will be needed to build the project:
1. [Cmake](https://cmake.org/download/) >= 3.22.1
2. Python 3 >= 3.12.3
3. Conan2 >= 2.5.0 (Install with pip)
4. Go >= 1.22.3
5. MSVC >= 19.40.33813 with Windows SDK 10.0.26100.0 or GCC 13 for Linux

The backend server must be built using the commands:

1) `python3 conanfile.py -rel` for Release build type
2) `python3 conanfile.py -deb` for Debug build type

in folver `core/server/`.

The client server must be built using the command: `go build server.go` in folder `core/client/server/`.
And the cli can be built with the command: `go build cli.go` in folder `core/client/cli/`.

After the backend server is built, the `build/` folder will appear, and next to it the `bin/` folder, which will contain all executable files (server, cli and tests).

## How to run it?

Run backend executable file with name **FDESolverServer_Debug** in `Debug/` folder or **FDESolverServer_Release** in `Release/` folder,
if they were previously built with the appropriate build type.

Run the client executable file in the path you specified during the build.

Go to the browser at the link localhost:8002, and you should see the page as below.

## About this repository

|Path|Description|
|-|-|
|`core/client`|Go code with tools for visualizing the results of solving a problem and a frontend server that supports an interface for interacting with the solver|
|`core/server`|C++ backend code with query processing algorithm and solver itself|
|`core/server/fde_solver/src/mfdes.cpp`|Implementation of Matrix Fractional Differential Equations solver|
|`core/server/fde_solver/src/sfdes.cpp`|Implementation of Stochastic Fractional Differential Equations solver|
|`core/server/fde_solver/src/rules_impl.cpp`|A group of implementations of methods for solving fractional differential equations (Riemann-Liouville method and Grunwald-Letnikov method)|

## Demonstration
![](https://github.com/Igor743646/FDESolverServer/blob/master/images/FDESolver4.png)






