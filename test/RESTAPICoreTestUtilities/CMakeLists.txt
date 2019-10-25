cmake_minimum_required(VERSION 3.2)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find external dependencides
find_package(gtest)
find_package(TestUtilitiesInterface)

# Configure test project
set(REST_API_CORE_TEST_UTILITIES_PROJECT RESTAPICoreTestUtilities)
file(GLOB_RECURSE REST_API_CORE_TEST_UTILITIES_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE REST_API_CORE_TEST_UTILITIES_PROJECT_HDR "*.h")

set(EXCLUDE_DIR test_package)
foreach (TMP_PATH ${REST_API_CORE_TEST_UTILITIES_PROJECT_SRC})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM REST_API_CORE_TEST_UTILITIES_PROJECT_SRC ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

foreach (TMP_PATH ${REST_API_CORE_TEST_UTILITIES_PROJECT_HDR})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM REST_API_CORE_TEST_UTILITIES_PROJECT_HDR ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

add_library(${REST_API_CORE_TEST_UTILITIES_PROJECT} STATIC ${REST_API_CORE_TEST_UTILITIES_PROJECT_SRC} ${REST_API_CORE_TEST_UTILITIES_PROJECT_HDR})
target_link_libraries(${REST_API_CORE_TEST_UTILITIES_PROJECT} RESTAPICore gtest::gtest TestUtilitiesInterface::TestUtilitiesInterface)

#Configure source groups
foreach(FILE ${REST_API_CORE_TEST_UTILITIES_PROJECT_SRC} ${REST_API_CORE_TEST_UTILITIES_PROJECT_HDR}) 
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
