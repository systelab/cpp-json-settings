from conans import ConanFile, CMake, tools

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
    options = {"boost": ["1.66.0", "1.67.0"], "gtest": ["1.7.0", "1.8.1"]}
    default_options = {"boost":"1.67.0", "gtest":"1.8.1"}
    exports_sources = "*"

    def configure(self):
        self.options["boost"].shared = True
        self.options["RapidJSONAdapter"].gtest = self.options.gtest
        self.options["JSONAdapterTestUtilities"].gtest = self.options.gtest

    def requirements(self):
        self.requires(("boost/%s@conan/stable") % self.options.boost)

    def build_requirements(self):
        self.build_requires("RapidJSONAdapter/1.0.5@systelab/stable")
        self.build_requires("JSONAdapterTestUtilities/1.0.8@systelab/stable")
        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        else:
            self.build_requires("gtest/1.8.1@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("SettingValue.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SettingDefinition.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SettingDefinitionMgr.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SettingDefinitionMacros.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("ISettingsService.h", dst="include/JSONSettings", src="src/JSONSettings/Services")
        self.copy("SettingsService.h", dst="include/JSONSettings", src="src/JSONSettings/Services")
        self.copy("SettingsMacros.h", dst="include/JSONSettings", src="src/JSONSettings/Services")
        self.copy("*JSONSettings.lib", dst="lib", keep_path=False)
        self.copy("*JSONSettings.pdb", dst="lib", keep_path=False)
        self.copy("*JSONSettings.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
