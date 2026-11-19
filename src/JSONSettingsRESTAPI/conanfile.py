import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import copy

class JSONSettingsRESTAPIConan(ConanFile):
    name = "json-settings-restapi"
    description = "REST API endpoints for C++ JSON Configuration library"
    url = "https://github.com/systelab/cpp-json-settings"
    homepage = "https://github.com/systelab/cpp-json-settings"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "json", "settings", "utils")
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "Endpoints/*", "Helpers/*", "stdafx.h"
    required_conan_version = ">=2.0.0"

    def requirements(self):
        self.requires("rest-api-core/1.1.13", transitive_headers=True)
        self.requires(f"json-settings/{self.version}")

    def build_requirements(self):
        self.test_requires("caeser-cypher-encryption-adapter/1.2.1")
        self.test_requires("rapidjson-adapter/1.2.1")
        self.test_requires("json-adapter-test-utilities/1.2.1")
        self.test_requires("gtest/1.14.0")

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
        cmake.configure(build_script_folder=self.source_folder)
        cmake.build()

    def package(self):
        copy(self, "*.h", src=os.path.join(self.source_folder, "Endpoints"),
             dst=os.path.join(self.package_folder, "include", "JSONSettingsRESTAPI"), keep_path=False)
        lib = os.path.join(self.package_folder, "lib")
        for pattern in ("*JSONSettingsRESTAPI.lib", "*JSONSettingsRESTAPI.pdb", "*JSONSettingsRESTAPI.a"):
            copy(self, pattern, src=self.build_folder, dst=lib, keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["JSONSettingsRESTAPI"]
        self.cpp_info.set_property("cmake_file_name", "JSONSettingsRESTAPI")
        self.cpp_info.set_property("cmake_target_name", "JSONSettingsRESTAPI::JSONSettingsRESTAPI")
