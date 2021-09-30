import os
from conans import ConanFile, tools

class RESTAPICoreTestUtilitiesConan(ConanFile):
    name = "RESTAPICoreTestUtilities"
    description = "Test utilities for C++ REST API framework"
    url = "https://github.com/systelab/cpp-rest-api-core"
    homepage = "https://github.com/systelab/cpp-rest-api-core"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "rest", "api", "framework", "test", "utilities")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    options = {"gtest": ["1.7.0", "1.8.1", "1.10.0"], "openssl": ["1.0.2n", "1.0.2s", "1.1.1g", "1.1.1k"]}
    default_options = {"gtest":"1.10.0", "openssl":"1.1.1k"}

    def configure(self):
        self.options["RESTAPICore"].gtest = self.options.gtest
        self.options["RESTAPICore"].openssl = self.options.openssl

    def requirements(self):
        if self.options.gtest == "1.7.0":
            self.requires("gtest/1.7.0@systelab/stable")
        elif self.options.gtest == "1.8.1":
            self.requires("gtest/1.8.1")
        elif self.options.gtest == "1.10.0":
            self.requires("gtest/1.10.0#0c895f60b461f8fee0da53a84d659131")
        else:
            self.requires(f"gtest/{self.options.gtest}")

        if ("%s" % self.version) == "None":
            self.requires("RESTAPICore/%s@systelab/stable" % os.environ['VERSION'])
        else:
            self.requires("RESTAPICore/%s@systelab/stable" % self.version)

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.so*", dst="bin", src="lib")

    def package(self):
        self.copy("*.h", dst="include/RESTAPICoreTestUtilities", keep_path=True)
        self.copy("*RESTAPICoreTestUtilities.lib", dst="lib", keep_path=False)
        self.copy("*RESTAPICoreTestUtilities.pdb", dst="lib", keep_path=False)
        self.copy("*RESTAPICoreTestUtilities.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
