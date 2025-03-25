import os
from conans import ConanFile, tools, CMake

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
    exports_sources = "*", "!build*", "!*.yml", "!*.md", "!*.in", "!ci", "!.gitattributes", "!.gitignore", "!LICENSE"

    def requirements(self):
        self.requires("gtest/1.14.0#4372c5aed2b4018ed9f9da3e218d18b3")

        if ("%s" % self.version) == "None":
            self.requires(f"RESTAPICore/{os.environ['VERSION']}@systelab/{os.environ['CHANNEL']}")
        else:
            self.requires(f"RESTAPICore/{self.version}@systelab/{self.channel}")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

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
