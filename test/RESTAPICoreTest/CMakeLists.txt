cmake_minimum_required(VERSION 3.2)

# Enable testing module
enable_testing()

# Find external dependencides
set(GTEST_PACKAGE_NAME gtest)
find_package(${GTEST_PACKAGE_NAME})
if (NOT TARGET ${GTEST_PACKAGE_NAME}::${GTEST_PACKAGE_NAME})
    set(GTEST_PACKAGE_NAME GTest)
    find_package(${GTEST_PACKAGE_NAME})
endif()

find_package(WebServerAdapterTestUtilities)
find_package(JSONAdapterTestUtilities)
find_package(TimeAdapterTestUtilities)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure test project
set(REST_API_CORE_TEST_PROJECT RESTAPICoreTest)
file(GLOB_RECURSE REST_API_CORE_TEST_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE REST_API_CORE_TEST_PROJECT_HDR "*.h")
add_executable(${REST_API_CORE_TEST_PROJECT} ${REST_API_CORE_TEST_PROJECT_SRC} ${REST_API_CORE_TEST_PROJECT_HDR})
target_link_libraries(${REST_API_CORE_TEST_PROJECT} RESTAPICore RESTAPICoreTestUtilities
					  ${GTEST_PACKAGE_NAME}::${GTEST_PACKAGE_NAME}
					  WebServerAdapterTestUtilities::WebServerAdapterTestUtilities
					  JSONAdapterTestUtilities::JSONAdapterTestUtilities
					  TimeAdapterTestUtilities::TimeAdapterTestUtilities)
					  
target_include_directories(${REST_API_CORE_TEST_PROJECT} PRIVATE ${JSONAdapterTestUtilities_INCLUDE_DIRS})
#Configure source groups
foreach(FILE ${REST_API_CORE_TEST_PROJECT_SRC} ${REST_API_CORE_TEST_PROJECT_HDR}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# Register tests
add_test(NAME RESTAPICoreTest COMMAND ${REST_API_CORE_TEST_PROJECT})

# Additional target for code coverage analysis
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	include(CodeCoverage)
	setup_target_for_coverage(RESTAPICoreTestCoverage RESTAPICoreTest coverage)
endif()
