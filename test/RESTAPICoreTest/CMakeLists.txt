enable_testing()

# Configure preprocessor definitions
add_compile_options(-DBOOST_ALL_DYN_LINK -DSQLITE_HAS_CODEC)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure test project
set(REST_API_CORE_TEST_PROJECT RESTAPICoreTest)
file(GLOB_RECURSE REST_API_CORE_TEST_PROJECT_SRC "*.cpp")
file(GLOB_RECURSE REST_API_CORE_TEST_PROJECT_HDR "*.h")
add_executable(${REST_API_CORE_TEST_PROJECT} ${REST_API_CORE_TEST_PROJECT_SRC} ${REST_API_CORE_TEST_PROJECT_HDR})
target_link_libraries(${REST_API_CORE_TEST_PROJECT} gmock_main RESTAPICore RESTAPICoreTestUtilities RapidJSONAdapter JWTUtils ${CONAN_LIBS})

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
