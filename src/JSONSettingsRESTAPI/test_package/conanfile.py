from conans import ConanFile, CMake


class JSONSettingsRESTAPITestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package"
    required_conan_version = ">=1.33.1"

    def requirements(self):
        self.requires("RapidJSONAdapter/1.1.6@systelab/stable")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy('*.so*', dst='bin', src='lib')

    def test(self):
        cmake = CMake(self)
        cmake.test()
