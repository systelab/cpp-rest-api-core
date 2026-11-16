from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy

required_conan_version = ">=2.0.0"


class RESTAPICoreConan(ConanFile):
    name = "rest-api-core"
    description = "C++ REST API framework"
    url = "https://github.com/systelab/cpp-rest-api-core"
    homepage = "https://github.com/systelab/cpp-rest-api-core"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "rest", "api", "framework")
    license = "MIT"
    package_type = "static-library"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "src/*", "test/*"

    def requirements(self):
        self.requires("web-server-adapter-interface/2.0.3")
        self.requires("jwt-utils/1.2.5")
        self.requires("time-adapter/1.0.7")

    def build_requirements(self):
        self.test_requires("gtest/1.14.0")
        self.test_requires("test-utilities-interface/1.1.0")
        self.test_requires("web-server-adapter-test-utilities/2.0.3")
        self.test_requires("json-adapter-test-utilities/1.2.1")
        self.test_requires("time-adapter-test-utilities/1.0.7")

    def layout(self):
        cmake_layout(self)
        self.folders.build_folder_vars = ["settings.arch"]

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

        runtime_dst = f"{self.build_folder}/bin/{self.settings.build_type}"
        for dep in self.dependencies.values():
            for bindir in dep.cpp_info.bindirs:
                copy(self, "*.dll", bindir, runtime_dst, keep_path=False)
            for libdir in dep.cpp_info.libdirs:
                copy(self, "*.dylib*", libdir, runtime_dst, keep_path=False)
                copy(self, "*.so*", libdir, runtime_dst, keep_path=False)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        if self.settings.os == "Windows":
            libs_folder = f"{self.build_folder}/lib/{self.settings.build_type}"
        else:
            libs_folder = f"{self.build_folder}/lib"
        copy(self, "*.h", f"{self.source_folder}/src/RESTAPICore", f"{self.package_folder}/include/RESTAPICore")
        copy(self, "*RESTAPICore.lib", libs_folder, f"{self.package_folder}/lib", keep_path=False)
        copy(self, "*RESTAPICore.pdb", libs_folder, f"{self.package_folder}/lib", keep_path=False)
        copy(self, "*RESTAPICore.a", libs_folder, f"{self.package_folder}/lib", keep_path=False)

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "RESTAPICore")
        self.cpp_info.set_property("cmake_target_name", "RESTAPICore::RESTAPICore")
        self.cpp_info.libs = ["RESTAPICore"]
