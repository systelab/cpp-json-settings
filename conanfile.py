from conans import ConanFile, tools

class JSONSettingsConan(ConanFile):
    name = "JSONSettings"
    description = "C++ JSON Configuration library"
    url = "https://github.com/systelab/cpp-json-settings"
    homepage = "https://github.com/systelab/cpp-json-settings"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "json", "configuration", "settings")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    options = {"gtest": ["1.7.0", "1.8.1"], "OpenSSL": ["1.0.2n"]}
    default_options = {"gtest":"1.8.1", "OpenSSL":"1.0.2n"}

    def configure(self):
        self.options["RapidJSONAdapter"].gtest = self.options.gtest

    def requirements(self):
        self.requires("RapidJSONAdapter/1.0.3@systelab/stable")

    def build_requirements(self):
        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        else:
            self.build_requires("gtest/1.8.1@bincrafters/stable")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("*.h", dst="include/JSONSettings", src="src/JSONSettings")
        self.copy("*JSONSettings.lib", dst="lib", keep_path=False)
        self.copy("*JSONSettings.pdb", dst="lib", keep_path=False)
        self.copy("*JSONSettings.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
