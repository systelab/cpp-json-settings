import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import copy

class JSONSettingsConan(ConanFile):
    name = "json-settings"
    description = "C++ JSON Configuration library"
    url = "https://github.com/systelab/cpp-json-settings"
    homepage = "https://github.com/systelab/cpp-json-settings"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "json", "configuration", "settings")
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "cmake/CodeCoverage.cmake", "src/JSONSettings/*", "test/JSONSettingsTest/*"
    required_conan_version = ">=2.0.0"

    def requirements(self):
        self.requires("encryption-adapter-interface/1.2.1")
        self.requires("caeser-cypher-encryption-adapter/1.2.1")
        self.requires("rapidjson-adapter/1.2.1")
        self.requires("boost/1.85.0")

    def build_requirements(self):
        self.test_requires("encryption-adapter-test-utilities/1.2.1")
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
        model = os.path.join(self.source_folder, "src", "JSONSettings", "Model")
        services = os.path.join(self.source_folder, "src", "JSONSettings", "Services")
        include = os.path.join(self.package_folder, "include", "JSONSettings")
        for header in ("SettingsFile.h", "SettingPath.h", "SettingValue.h", "SettingDefinition.h",
                       "SettingDefinitionMgr.h", "SettingDefinitionMacros.h", "SecurityKey.h"):
            copy(self, header, src=model, dst=include)
        for header in ("ISettingsService.h", "SettingsService.h", "SettingsMacros.h"):
            copy(self, header, src=services, dst=include)
        lib = os.path.join(self.package_folder, "lib")
        for pattern in ("*JSONSettings.lib", "*JSONSettings.pdb", "*JSONSettings.a"):
            copy(self, pattern, src=self.build_folder, dst=lib, keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["JSONSettings"]
        self.cpp_info.set_property("cmake_file_name", "JSONSettings")
        self.cpp_info.set_property("cmake_target_name", "JSONSettings::JSONSettings")
