import os
from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class JSONSettingsRESTAPITestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    required_conan_version = ">=2.0.0"

    def requirements(self):
        self.requires(self.tested_reference_str)
        self.requires("rapidjson-adapter/1.2.1")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()
        bin_dst = os.path.join(self.build_folder, "bin", str(self.settings.build_type))
        for dep in self.dependencies.values():
            for src in dep.cpp_info.bindirs + dep.cpp_info.libdirs:
                copy(self, "*.dll", src=src, dst=bin_dst)
                copy(self, "*.dylib*", src=src, dst=os.path.join(self.build_folder, "bin"))
                copy(self, "*.so*", src=src, dst=os.path.join(self.build_folder, "bin"))

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if can_run(self):
            cmake = CMake(self)
            cmake.test()
