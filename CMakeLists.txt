cmake_minimum_required(VERSION 3.2)

project(RESTAPICore)

# Configure environment
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup google_test
include(ConfigureGoogleTest)
configure_google_test()

# Setup 3rd party libraries through conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Setup extern repositories
include(GitCloneRepository)
git_clone_repository(URL https://github.com/systelab/cpp-test-utilities.git                DIR extern/TestUtilities               BRANCH v1.0.0)
git_clone_repository(URL https://github.com/systelab/cpp-json-adapter.git                  DIR extern/JSONAdapter                 BRANCH v1.0.0)
git_clone_repository(URL https://github.com/systelab/cpp-rapidjson-json-adapter.git        DIR extern/RapidJSONAdapter            BRANCH v1.0.0)
git_clone_repository(URL https://github.com/systelab/cpp-webserver-adapter.git             DIR extern/WebServerAdapter            BRANCH v1.0.0)
git_clone_repository(URL https://github.com/systelab/cpp-jwt-utils.git                     DIR extern/JWTUtils                    BRANCH v1.0.0)

# Configure include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/test)
include_directories(${CMAKE_SOURCE_DIR}/extern/JWTUtils/src)

# Add extern projects
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/JSONAdapter/JSONAdapterTestUtilities)
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/RapidJSONAdapter/src/RapidJSONAdapter)
#add_subdirectory(${CMAKE_SOURCE_DIR}/extern/JWTUtils/src/JWTUtils)

# Add library projects
add_subdirectory(${CMAKE_SOURCE_DIR}/src/RESTAPICore)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/RESTAPICoreTest)

