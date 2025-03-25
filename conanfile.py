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
    exports_sources = "*", "!build*"
    required_conan_version = ">=1.33.1"

    def requirements(self):
        self.requires("EncryptionAdapterInterface/1.2.0@systelab/stable")
        self.requires("CaeserCypherEncryptionAdapter/1.2.0@systelab/stable")
        self.requires("RapidJSONAdapter/1.1.7@systelab/stable")
        self.requires("boost/1.85.0#0734cd0dd8fe650aa3ae64bb51bca54d")

    def build_requirements(self):
        self.build_requires("EncryptionAdapterTestUtilities/1.2.0@systelab/stable")
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
