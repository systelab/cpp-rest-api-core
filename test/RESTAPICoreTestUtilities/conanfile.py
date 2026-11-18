import os
from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.files import copy

required_conan_version = ">=2.0.0"


class RESTAPICoreTestUtilitiesConan(ConanFile):
    name = "rest-api-core-test-utilities"
    description = "Test utilities for C++ REST API framework"
    url = "https://github.com/systelab/cpp-rest-api-core"
    homepage = "https://github.com/systelab/cpp-rest-api-core"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "rest", "api", "framework", "test", "utilities")
    license = "MIT"
    package_type = "static-library"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "stdafx.h", "Builders/*", "Comparators/*", "Mocks/*", "Stubs/*"

    def requirements(self):
        if self.version is None:
            raise ConanInvalidConfiguration("A version is required")
        
        self.requires("gtest/1.14.0", transitive_headers=True)
        self.requires(f"rest-api-core/{self.version}", transitive_headers=True)

    def layout(self):
        self.folders.build = os.path.join("..", "..", "build", f"{self.settings.arch}-{str(self.settings.build_type).lower()}")

    def package(self):
        if self.settings.os == "Windows":
            libs_folder = os.path.join(self.build_folder, "lib", str(self.settings.build_type))
        else:
            libs_folder = os.path.join(self.build_folder, "lib")
        copy(self, "*.h", self.source_folder, os.path.join(self.package_folder, "include", "RESTAPICoreTestUtilities"), excludes="test_package/*")
        copy(self, "*RESTAPICoreTestUtilities.lib", libs_folder, os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*RESTAPICoreTestUtilities.pdb", libs_folder, os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*RESTAPICoreTestUtilities.a", libs_folder, os.path.join(self.package_folder, "lib"), keep_path=False)

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "RESTAPICoreTestUtilities")
        self.cpp_info.set_property("cmake_target_name", "RESTAPICoreTestUtilities::RESTAPICoreTestUtilities")
        self.cpp_info.libs = ["RESTAPICoreTestUtilities"]
