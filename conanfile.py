# /usr/bin/python3
import os
from conan import ConanFile
from conan.tools.build import can_run

class TraactPackage(ConanFile):
    python_requires = "traact_base/0.0.0@traact/latest"
    python_requires_extend = "traact_base.TraactPackageCmake"

    name = "traact_component_cereal"
    version = "0.0.0"
    description = "Cereal based serialization for traact"
    url = "https://github.com/traact/traact_component_cereal.git"
    license = "MIT"
    author = "Frieder Pankratz"

    settings = "os", "compiler", "build_type", "arch"
    compiler = "cppstd"

    exports_sources = "src/*", "include/*", "tests/*", "CMakeLists.txt"

    options = {
        "shared": [True, False],
        "trace_logs_in_release": [True, False]
    }

    default_options = {
        "shared": True,
        "trace_logs_in_release": True
    }    

    def requirements(self):
        self.requires("traact_spatial/0.0.0@traact/latest")
        self.requires("traact_vision/0.0.0@traact/latest")
        self.requires("cereal/1.3.2", transitive_headers=True, transitive_libs=True)
        self.requires("cppfs/1.3.0@camposs/stable")

    def _after_package_info(self):
        self.cpp_info.libs = ["traact_component_cereal"]
