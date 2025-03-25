import os
from conans import ConanFile, CMake, tools

class JSONSettingsRESTAPIConan(ConanFile):
    name = "JSONSettingsRESTAPI"
    description = "REST API endpoints for C++ JSON Configuration library"
    url = "https://github.com/systelab/cpp-json-settings"
    homepage = "https://github.com/systelab/cpp-json-settings"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "json", "settings", "utils")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*","!build","!test_package"
    required_conan_version = ">=1.33.1"

    def requirements(self):
        self.requires("RESTAPICore/1.1.9@systelab/stable")
        self.requires("zlib/1.3.1")
        self.requires(f"JSONSettings/{os.environ['VERSION']}@systelab/{os.environ['CHANNEL']}")

    def build_requirements(self):
        self.build_requires("CaeserCypherEncryptionAdapter/1.2.0@systelab/stable")
        self.build_requires("RapidJSONAdapter/1.1.7@systelab/stable", override=True)
        self.build_requires("JSONAdapterTestUtilities/1.1.6@systelab/stable")
        self.build_requires("gtest/1.14.0#4372c5aed2b4018ed9f9da3e218d18b3")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("*.h", src="Endpoints", dst="include/JSONSettingsRESTAPI", keep_path=False)
        self.copy("*JSONSettingsRESTAPI.lib", dst="lib", keep_path=False)
        self.copy("*JSONSettingsRESTAPI.pdb", dst="lib", keep_path=False)
        self.copy("*JSONSettingsRESTAPI.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
