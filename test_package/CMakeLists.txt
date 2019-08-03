cmake_minimum_required(VERSION 3.2)

project(RESTAPICorePackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find external dependencies
find_package(RESTAPICore)

# Configure test package project
set(REST_API_CORE_PACKAGE_TEST_PROJECT RESTAPICorePackageTest)
add_executable(${REST_API_CORE_PACKAGE_TEST_PROJECT} RESTAPICoreExample.cpp)
target_link_libraries(${REST_API_CORE_PACKAGE_TEST_PROJECT} RESTAPICore::RESTAPICore)

# Register tests
enable_testing()
add_test(NAME RESTAPICorePackageTest COMMAND ${REST_API_CORE_PACKAGE_TEST_PROJECT})
