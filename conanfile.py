from conan import ConanFile
from conan.tools.cmake import cmake_layout


class FDEWebSolverRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("protobuf/3.21.12")
        self.requires("grpc/1.54.3")
        self.requires("antlr4-cppruntime/4.13.1")

    def layout(self):
        cmake_layout(self)