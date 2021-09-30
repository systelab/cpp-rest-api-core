import os
from conans import ConanFile, CMake, tools


class RESTAPICoreTestUtilitiesTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package"
    options = {"gtest": ["1.7.0", "1.8.1", "1.10.0"], "openssl": ["1.0.2n", "1.0.2s", "1.1.1g", "1.1.1k"]}
    default_options = {"gtest":"1.10.0", "openssl":"1.1.1k"}

    def configure(self):
        self.options["RESTAPICoreTestUtilities"].gtest = self.options.gtest
        self.options["RESTAPICoreTestUtilities"].openssl = self.options.openssl

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
