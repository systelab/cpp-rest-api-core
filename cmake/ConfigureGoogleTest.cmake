cmake_minimum_required(VERSION 3.2)

macro(configure_google_test)
    # Download and unpack googletest at configure time
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GoogleTest.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
                    RESULT_VARIABLE result
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    if(result)
      message(FATAL_ERROR CMake step for googletest failed ${result})
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
                    RESULT_VARIABLE result
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    if(result)
      message(FATAL_ERROR Build step for googletest failed ${result})
    endif()

	# Prevent overriding the parent project's compiler/linker settings on Windows
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                     ${CMAKE_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)
					 
	# Add GoogleTest to includes
	include_directories("${gtest_SOURCE_DIR}/include")
	include_directories("${gmock_SOURCE_DIR}/include")
	
	# Include cmake module for GoogleTest
	include(GoogleTest)
endmacro()
