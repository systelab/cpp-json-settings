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
    options = {"boost": ["1.66.0", "1.67.0", "1.72.0"], "openssl": ["1.0.2n", "1.0.2s", "1.1.1g"], "gtest": ["1.7.0", "1.8.1","1.10.0"]}
    default_options = {"boost":"1.72.0", "openssl": "1.1.1g", "gtest":"1.10.0"}
    exports_sources = "*","!build","!test_package"
    required_conan_version = ">=1.33.1"

    def configure(self):
        self.options["RESTAPICore"].openssl = self.options.openssl
        self.options["RESTAPICore"].gtest = self.options.gtest
        self.options["JSONSettings"].boost = self.options.boost
        self.options["JSONSettings"].gtest = self.options.gtest

        self.options["CaeserCypherEncryptionAdapter"].gtest = self.options.gtest
        self.options["RapidJSONAdapter"].gtest = self.options.gtest
        self.options["JSONAdapterTestUtilities"].gtest = self.options.gtest

    def requirements(self):
        self.requires("RESTAPICore/1.1.9@systelab/stable")
        self.requires("zlib/1.2.11")

        if ("%s" % self.version) == "None":
            channel = os.environ['CHANNEL'] if "CHANNEL" in os.environ else "stable"
            self.requires(f"JSONSettings/{os.environ['VERSION']}@systelab/{channel}")
        else:
            self.requires(f"JSONSettings/{self.version}@systelab/{self.channel}")

    def build_requirements(self):
        self.build_requires("CaeserCypherEncryptionAdapter/1.1.0@systelab/stable")
        self.build_requires("RapidJSONAdapter/1.1.6@systelab/stable")
        self.build_requires("JSONAdapterTestUtilities/1.1.5@systelab/stable")
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
