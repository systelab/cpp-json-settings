import os
from conans import ConanFile, CMake, tools


class JSONSettingsTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package"
    options = {"boost": ["1.66.0", "1.67.0"], "gtest": ["1.7.0", "1.8.1"]}
    default_options = {"boost":"1.67.0", "gtest":"1.8.1"}

    def configure(self):
        self.options["JSONSettings"].boost = self.options.boost
        self.options["JSONSettings"].gtest = self.options.gtest

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        cmake = CMake(self)
        cmake.test()
