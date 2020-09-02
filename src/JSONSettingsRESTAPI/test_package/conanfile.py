from conans import ConanFile, CMake


class JSONSettingsRESTAPITestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package"
    options = {"boost": ["1.66.0", "1.67.0", "1.72.0"], "openssl": ["1.0.2n", "1.0.2s", "1.1.1g"], "gtest": ["1.7.0", "1.8.1", "1.10.0"]}
    default_options = {"boost":"1.72.0", "openssl": "1.1.1g", "gtest":"1.10.0"}

    def configure(self):
        self.options["JSONSettingsRESTAPI"].boost = self.options.boost
        self.options["JSONSettingsRESTAPI"].openssl = self.options.openssl
        self.options["JSONSettingsRESTAPI"].gtest = self.options.gtest

    def requirements(self):
        self.requires("RapidJSONAdapter/1.0.8@systelab/stable")

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
