from conans import ConanFile, tools

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

    def requirements(self):
        self.requires("JWTUtils/1.0.2@systelab/stable")
        self.requires("WebServerAdapter/1.0.1@systelab/stable")

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
