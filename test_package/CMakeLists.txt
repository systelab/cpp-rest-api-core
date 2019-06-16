cmake_minimum_required(VERSION 3.2)

project(RESTAPICorePackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup 3rd party libraries through conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Configure test package project
set(REST_API_CORE_PACKAGE_TEST_PROJECT RESTAPICorePackageTest)
add_executable(${REST_API_CORE_PACKAGE_TEST_PROJECT} RESTAPICoreExample.cpp)
target_link_libraries(${REST_API_CORE_PACKAGE_TEST_PROJECT} ${CONAN_LIBS})
