from conans import ConanFile, CMake, tools

class RESTAPICoreConan(ConanFile):
    name = "RESTAPICore"
    description = "C++ REST API framework"
    url = "https://github.com/systelab/cpp-rest-api-core"
    homepage = "https://github.com/systelab/cpp-rest-api-core"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "rest", "api", "framework")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    options = {"gtest": ["1.7.0", "1.8.1", "1.10.0"], "openssl": ["1.0.2n", "1.0.2s", "1.1.1g"]}
    default_options = {"gtest":"1.10.0", "openssl":"1.1.1g"}
    exports_sources = "*"

    def configure(self):
        self.options["WebServerAdapterTestUtilities"].gtest = self.options.gtest
        self.options["JSONAdapterTestUtilities"].gtest = self.options.gtest
        self.options["TimeAdapter"].gtest = self.options.gtest
        self.options["TimeAdapterTestUtilities"].gtest = self.options.gtest
        self.options["JWTUtils"].gtest = self.options.gtest
        self.options["JWTUtils"].openssl = self.options.openssl

    def requirements(self):
        self.requires("WebServerAdapterInterface/1.1.7@systelab/stable")
        self.requires("JWTUtils/1.1.3@systelab/stable")
        self.requires("TimeAdapter/1.0.3@systelab/stable")

    def build_requirements(self):
        self.build_requires("TestUtilitiesInterface/1.0.4@systelab/stable")
        self.build_requires("WebServerAdapterTestUtilities/1.1.7@systelab/stable")
        self.build_requires("JSONAdapterTestUtilities/1.0.11@systelab/stable")
        self.build_requires("TimeAdapterTestUtilities/1.0.3@systelab/stable")
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
        self.copy("*.dylib*", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.so*", dst=("bin/%s" % self.settings.build_type), src="lib")

    def package(self):
        self.copy("*.h", dst="include/RESTAPICore", src="src/RESTAPICore")
        self.copy("*RESTAPICore.lib", dst="lib", keep_path=False)
        self.copy("*RESTAPICore.pdb", dst="lib", keep_path=False)
        self.copy("*RESTAPICore.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
