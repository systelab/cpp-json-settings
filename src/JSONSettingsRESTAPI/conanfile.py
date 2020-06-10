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
    options = {"boost": ["1.66.0", "1.67.0", "1.71.0"], "openssl": ["1.0.2n", "1.0.2s", "1.1.1g"], "gtest": ["1.7.0", "1.8.1","1.10.0"]}
    default_options = {"boost":"1.71.0", "openssl": "1.1.1g", "gtest":"1.10.0"}
    exports_sources = "*","!build","!test_package"

    def configure(self):
        self.options["RESTAPICore"].openssl = self.options.openssl
        self.options["RESTAPICore"].gtest = self.options.gtest
        self.options["JSONSettings"].boost = self.options.boost
        self.options["JSONSettings"].gtest = self.options.gtest
        self.options["JSONAdapterTestUtilities"].gtest = self.options.gtest

    def requirements(self):
        self.requires("RESTAPICore/1.1.1@systelab/stable")
        self.requires("zlib/1.2.11")

        if ("%s" % self.version) == "None":
            self.requires("JSONSettings/%s@systelab/stable" % os.environ['VERSION'])
        else:
            self.requires("JSONSettings/%s@systelab/stable" % self.version)

    def build_requirements(self):
        self.build_requires("RapidJSONAdapter/1.0.8@systelab/stable")
        self.build_requires("JSONAdapterTestUtilities/1.0.11@systelab/stable")
        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        elif self.options.gtest == "1.8.1":
            self.build_requires("gtest/1.8.1")
        else:
            self.build_requires("gtest/1.10.0")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("*.h", src="Endpoints", dst="include/JSONSettingsRESTAPI", keep_path=False)
        self.copy("*JSONSettingsRESTAPI.lib", dst="lib", keep_path=False)
        self.copy("*JSONSettingsRESTAPI.pdb", dst="lib", keep_path=False)
        self.copy("*JSONSettingsRESTAPI.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
