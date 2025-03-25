import os
from conans import ConanFile, CMake, tools

class JSONSettingsTestUtilitiesConan(ConanFile):
    name = "JSONSettingsTestUtilities"
    description = "Test utilities for C++ JSON Configuration library"
    url = "https://github.com/systelab/cpp-json-settings"
    homepage = "https://github.com/systelab/cpp-json-settings"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "json", "settings", "utils")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*"
    required_conan_version = ">=1.33.1"

    def requirements(self):
        self.requires("gtest/1.14.0#4372c5aed2b4018ed9f9da3e218d18b3")

        if ("%s" % self.version) == "None":
            self.requires(f"JSONSettings/{os.environ['VERSION']}@systelab/{os.environ['CHANNEL']}")
        else:
            self.requires(f"JSONSettings/{self.version}@systelab/{self.channel}")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

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
