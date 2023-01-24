# /usr/bin/python3
import os
from conans import ConanFile, CMake, tools


class TraactPackage(ConanFile):
    python_requires = "traact_run_env/1.0.0@traact/latest"
    python_requires_extend = "traact_run_env.TraactPackageCmake"

    name = "traact_component_cereal"
    description = "Cereal based serialization for traact"
    url = "https://github.com/traact/traact_component_cereal.git"
    license = "MIT"
    author = "Frieder Pankratz"

    generators = "cmake"
    settings = "os", "compiler", "build_type", "arch"
    compiler = "cppstd"

    exports_sources = "src/*", "tests/*", "CMakeLists.txt"

    def requirements(self):
        self.traact_requires("traact_spatial", "latest")
        self.traact_requires("traact_vision", "latest")
        self.requires("cereal/1.3.0")
        if self.options.with_tests:
            self.requires("gtest/cci.20210126")
