import os
from conans import ConanFile, tools

class JSONSettingsTestUtilitiesConan(ConanFile):
    name = "JSONSettingsTestUtilities"
    description = "Test utilities for C++ JSON Configuration library"
    url = "https://github.com/systelab/cpp-json-settings"
    homepage = "https://github.com/systelab/cpp-json-settings"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "jwt", "utils", "security")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    options = {"boost": ["1.66.0", "1.67.0"], "gtest": ["1.7.0", "1.8.1"]}
    default_options = {"boost":"1.67.0", "gtest":"1.8.1"}

    def configure(self):
        self.options["JSONSettings"].boost = self.options.boost
        self.options["JSONSettings"].gtest = self.options.gtest

    def requirements(self):
        if self.options.gtest == "1.7.0":
            self.requires("gtest/1.7.0@systelab/stable")
        else:
            self.requires("gtest/1.8.1@bincrafters/stable")

        if ("%s" % self.version) == "None":
            self.requires("JSONSettings/%s@systelab/stable" % os.environ['VERSION'])
        else:
            self.requires("JSONSettings/%s@systelab/stable" % self.version)

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("*.h", dst="include/JSONSettingsTestUtilities", keep_path=True)
        self.copy("*JSONSettingsTestUtilities.lib", dst="lib", keep_path=False)
        self.copy("*JSONSettingsTestUtilities.pdb", dst="lib", keep_path=False)
        self.copy("*JSONSettingsTestUtilities.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
