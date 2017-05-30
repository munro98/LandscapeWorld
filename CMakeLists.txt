
cmake_minimum_required(VERSION 3.1)

set(LANDSCAPE_WORLD "LandscapeWorld" CACHE STRING "Landscape World")

#Project
project("${LANDSCAPE_WORLD}" CXX C)

#Enable IDE Project Folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)


#Force Output Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


#add_definitions("-std=c++11")
find_package (Threads)

#Find OpenGL
find_package(OpenGL REQUIRED)



#Include GLFW
add_subdirectory("${PROJECT_SOURCE_DIR}/ext/glfw")
include_directories("${PROJECT_SOURCE_DIR}/ext/glfw/include")

#Include GLEW
add_subdirectory("${PROJECT_SOURCE_DIR}/ext/glew-2.0.0")


#Include IMGUI Subproject
add_subdirectory("${PROJECT_SOURCE_DIR}/ext/imgui")

#Include GLM header library
include_directories("${PROJECT_SOURCE_DIR}/ext/glm")



add_subdirectory("${PROJECT_SOURCE_DIR}/ext/stb_image")
include_directories("${PROJECT_SOURCE_DIR}/ext/stb_image")


# Set Compiler Flags
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# Full normal warnings
	add_compile_options(/W4)
	# Ignore C4800: forcing X to bool (performance warning)
	add_compile_options(/wd4800)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# C++14
	add_compile_options(-std=c++1y)
	# Full normal warnings
	add_compile_options(-Wall -Wextra -pedantic)
	# Promote missing return to error
	add_compile_options(-Werror=return-type)
	# enable coloured output if gcc >= 4.9
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
		add_compile_options(-fdiagnostics-color)
	endif()
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
	# C++14
	add_compile_options(-std=c++1y)
	#add_compile_options(-pthread)
	# Full normal warnings
	add_compile_options(-Wall -Wextra -pedantic)
	# Promote missing return to error
	add_compile_options(-Werror=return-type)
endif()


#Add source files
add_subdirectory(src) # Primary source files
add_subdirectory(res) # Resources like shaders (show up in IDE)
set_property(TARGET ${LANDSCAPE_WORLD} PROPERTY FOLDER "LANDSCAPE_WORLD")






