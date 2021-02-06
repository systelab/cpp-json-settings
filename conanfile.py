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
    options = {"boost": ["1.66.0", "1.67.0", "1.72.0"], "gtest": ["1.7.0", "1.8.1", "1.10.0"]}
    default_options = {"boost":"1.72.0", "gtest":"1.10.0"}
    exports_sources = "*"
    required_conan_version = ">=1.33.1"

    def configure(self):
        self.options["boost"].shared = True
        self.options["RapidJSONAdapter"].gtest = self.options.gtest
        self.options["JSONAdapterTestUtilities"].gtest = self.options.gtest
        self.options["EncryptionAdapterTestUtilities"].gtest = self.options.gtest
        self.options["CaeserCypherEncryptionAdapter"].gtest = self.options.gtest

    def requirements(self):
        self.requires("EncryptionAdapterInterface/1.0.3@systelab/stable")
        if self.options.boost == "1.66.0":
            self.requires("boost/1.66.0@conan/stable")
        elif self.options.boost == "1.67.0":
            self.requires("boost/1.67.0@conan/stable")
        else:
            self.requires(("boost/%s") % self.options.boost)

    def build_requirements(self):
        self.build_requires("CaeserCypherEncryptionAdapter/1.0.2@systelab/stable")
        self.build_requires("RapidJSONAdapter/1.1.0@systelab/stable")
        self.build_requires("EncryptionAdapterTestUtilities/1.0.3@systelab/stable")
        self.build_requires("JSONAdapterTestUtilities/1.1.0@systelab/stable")
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
        self.copy("SettingsFile.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SettingPath.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SettingValue.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SettingDefinition.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SettingDefinitionMgr.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SettingDefinitionMacros.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("SecurityKey.h", dst="include/JSONSettings", src="src/JSONSettings/Model")
        self.copy("ISettingsService.h", dst="include/JSONSettings", src="src/JSONSettings/Services")
        self.copy("SettingsService.h", dst="include/JSONSettings", src="src/JSONSettings/Services")
        self.copy("SettingsMacros.h", dst="include/JSONSettings", src="src/JSONSettings/Services")
        self.copy("*JSONSettings.lib", dst="lib", keep_path=False)
        self.copy("*JSONSettings.pdb", dst="lib", keep_path=False)
        self.copy("*JSONSettings.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
