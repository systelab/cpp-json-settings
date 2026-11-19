import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import copy

class JSONSettingsTestUtilitiesConan(ConanFile):
    name = "json-settings-test-utilities"
    description = "Test utilities for C++ JSON Configuration library"
    url = "https://github.com/systelab/cpp-json-settings"
    homepage = "https://github.com/systelab/cpp-json-settings"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "json", "settings", "utils")
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "Mocks/*", "Stubs/*", "stdafx.h"
    required_conan_version = ">=2.0.0"

    def requirements(self):
        self.requires("gtest/1.14.0")
        self.requires(f"json-settings/{self.version}")

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()
        for dep in self.dependencies.values():
            for src in dep.cpp_info.bindirs + dep.cpp_info.libdirs:
                copy(self, "*.dll", src=src, dst=os.path.join(self.build_folder, "bin"))
                copy(self, "*.dylib*", src=src, dst=os.path.join(self.build_folder, "bin"))
                copy(self, "*.so*", src=src, dst=os.path.join(self.build_folder, "bin"))

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=self.source_folder)
        cmake.build()

    def package(self):
        copy(self, "*.h", src=self.source_folder,
             dst=os.path.join(self.package_folder, "include", "JSONSettingsTestUtilities"), keep_path=True)
        lib = os.path.join(self.package_folder, "lib")
        for pattern in ("*JSONSettingsTestUtilities.lib", "*JSONSettingsTestUtilities.pdb", "*JSONSettingsTestUtilities.a"):
            copy(self, pattern, src=self.build_folder, dst=lib, keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["JSONSettingsTestUtilities"]
        self.cpp_info.set_property("cmake_file_name", "JSONSettingsTestUtilities")
        self.cpp_info.set_property("cmake_target_name", "JSONSettingsTestUtilities::JSONSettingsTestUtilities")
