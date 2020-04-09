cmake_minimum_required(VERSION 3.2)

# Find external dependencides
find_package(WebServerAdapterInterface)
find_package(JWTUtils)
find_package(TimeAdapter)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure RESTAPICore static library
set(REST_API_CORE RESTAPICore)
file(GLOB_RECURSE REST_API_CORE_SRC "*.cpp")
file(GLOB_RECURSE REST_API_CORE_HDR "*.h")
add_library(${REST_API_CORE} STATIC ${REST_API_CORE_SRC} ${REST_API_CORE_HDR})
target_link_libraries(${REST_API_CORE} WebServerAdapterInterface::WebServerAdapterInterface
                                       JWTUtils::JWTUtils
                                       TimeAdapter::TimeAdapter)

#Configure source groups
foreach(FILE ${REST_API_CORE_SRC} ${REST_API_CORE_HDR}) 
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

