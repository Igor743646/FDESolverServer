import os
import argparse

from argparse import ArgumentParser
from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake


class FDESolverServer(ConanFile):
    name = "FDESolverServer"
    description = "Grpc Server for solving fractional derivative equations"
    homepage = "https://github.com/Igor743646/FDESolverServer"
    version = "1.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    build_policy = "missing"

    def requirements(self):
        self.requires("catch2/3.6.0")
        self.requires("protobuf/3.21.12")
        self.requires("grpc/1.54.3")
        self.requires("antlr4-cppruntime/4.13.1")
        self.requires("boost/1.85.0")

    def layout(self):
        if self.settings.os == "Windows":
            cmake_layout(self, build_folder=f"build/{self.settings.build_type}")
        if self.settings.os == "Linux":
            cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={"CMAKE_BUILD_TYPE" : self.settings.build_type})
        cmake.build()


def ParseArguments() -> argparse.Namespace:
    parser = ArgumentParser()
    parser.add_argument("-deb", "--debug", action="store_true", 
                        help="build with Debug build type")
    parser.add_argument("-rel", "--release", action="store_true", 
                        help="build with Release build type")

    return parser.parse_args()


def main():
    args = ParseArguments()
    if args.debug:
        os.system("conan build . -s build_type=Debug")
    if args.release:
        os.system("conan build . -s build_type=Release")


if __name__ == "__main__":
    main()
