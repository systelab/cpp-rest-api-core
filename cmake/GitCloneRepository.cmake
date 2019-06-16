cmake_minimum_required(VERSION 3.2)

include(CMakeParseArguments)

macro(git_clone_repository)

	# Parse arguments
    set(gitCloneRepositoryArgs URL DIR BRANCH)
    cmake_parse_arguments(GIT_CLONE_REPOSITORY "" "${gitCloneRepositoryArgs}" "" ${ARGN} )

	# Configure repository to clone
	message(STATUS "===========================================================")
	message(STATUS "GIT CLONE REPOSITORY")
	message(STATUS "===========================================================")
	message(STATUS "URL: ${GIT_CLONE_REPOSITORY_URL}")
	message(STATUS "DIR: ${GIT_CLONE_REPOSITORY_DIR}")
	message(STATUS "BRANCH: ${GIT_CLONE_REPOSITORY_BRANCH}")

	# Check if git is available
	find_package(Git)
	if(NOT GIT_FOUND)
		message(FATAL_ERROR "git not found!")
	endif()

	# Clone the repository
	if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${GIT_CLONE_REPOSITORY_DIR})
		message(STATUS "${GIT_CLONE_REPOSITORY_DIR} directory not found, cloning...")
		execute_process(
				COMMAND             ${GIT_EXECUTABLE} clone ${GIT_CLONE_REPOSITORY_URL} ${GIT_CLONE_REPOSITORY_DIR}
				WORKING_DIRECTORY   ${CMAKE_SOURCE_DIR}
				OUTPUT_VARIABLE     git_output)
	else()
		message(STATUS "${GIT_CLONE_REPOSITORY_DIR} found, pulling...")
		execute_process(
                COMMAND             ${GIT_EXECUTABLE} pull origin master
                WORKING_DIRECTORY   ${CMAKE_SOURCE_DIR}/${GIT_CLONE_REPOSITORY_DIR}
                OUTPUT_VARIABLE     git_output)
	endif()

	# Checkout the right branch
	execute_process(
			COMMAND				${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
			WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}/${GIT_CLONE_REPOSITORY_DIR}
			OUTPUT_VARIABLE 	GIT_CLONE_REPOSITORY_CURRENT_BRANCH
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	
	if (NOT ${GIT_CLONE_REPOSITORY_BRANCH} STREQUAL ${GIT_CLONE_REPOSITORY_CURRENT_BRANCH})
		message(STATUS "Checking out ${GIT_CLONE_REPOSITORY_BRANCH} branch...")
		execute_process(
				COMMAND             ${GIT_EXECUTABLE} checkout ${GIT_CLONE_REPOSITORY_BRANCH}
				WORKING_DIRECTORY   ${CMAKE_SOURCE_DIR}/${GIT_CLONE_REPOSITORY_DIR}
				OUTPUT_VARIABLE     git_output)
	endif()

	# Add cloned folder into includes
	include_directories(${CMAKE_SOURCE_DIR}/${GIT_CLONE_REPOSITORY_DIR})
	
	message(STATUS "===========================================================")
endmacro()
