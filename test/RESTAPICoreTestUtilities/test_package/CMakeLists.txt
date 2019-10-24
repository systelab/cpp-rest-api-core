cmake_minimum_required(VERSION 3.2)

project(RESTAPICoreTestUtilitiesPackageTestProject)

# Configure environment
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find tested package
find_package(RESTAPICoreTestUtilities)

# Configure test package project
set(REST_API_CORE_TEST_UTILITIES_PACKAGE_TEST_PROJECT RESTAPICoreTestUtilitiesPackageTest)
add_executable(${REST_API_CORE_TEST_UTILITIES_PACKAGE_TEST_PROJECT} RESTAPICoreTestUtilitiesExample.cpp)
target_link_libraries(${REST_API_CORE_TEST_UTILITIES_PACKAGE_TEST_PROJECT} RESTAPICoreTestUtilities::RESTAPICoreTestUtilities)

# Register test
enable_testing()
add_test(NAME RESTAPICoreTestUtilitiesPackageTest COMMAND ${REST_API_CORE_TEST_UTILITIES_PACKAGE_TEST_PROJECT})
