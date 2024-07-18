from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake


class FDESolverServerRecipe(ConanFile):
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
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
