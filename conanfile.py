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
    exports_sources = "*", "!test/RESTAPICoreTestUtilities", "!build-*", "!*.yml", "!*.md", "!*.in", "!ci", "!.gitattributes", "!.gitignore", "!LICENSE"

    def configure(self):
        self.options["openssl"].shared = True

    def requirements(self):
        self.requires("WebServerAdapterInterface/2.0.2@systelab/stable")
        self.requires("JWTUtils/1.1.10@systelab/stable")
        self.requires("TimeAdapter/1.0.6@systelab/stable")
        self.requires("gtest/1.14.0#4372c5aed2b4018ed9f9da3e218d18b3", private=True)
        self.requires("TestUtilitiesInterface/1.0.8@systelab/stable", private=True)
        self.requires("WebServerAdapterTestUtilities/2.0.2@systelab/stable", private=True)
        self.requires("JSONAdapterTestUtilities/1.1.5@systelab/stable", private=True)
        self.requires("TimeAdapterTestUtilities/1.0.6@systelab/stable", private=True)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
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
