cmake_minimum_required(VERSION 3.17)
set(PROJECT_NAME OpenGLRenderLib)
project(${PROJECT_NAME})

# CMAKE_DIR to find external dependecies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(RESOURCE_INSTALL_DIR "" CACHE PATH "Path to install resources to (leave empty for running uninstalled)")

# glfw
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(third_party/glfw)
include_directories(third_party/glfw/include)

# imgui
add_subdirectory(third_party/imgui)
include_directories(third_party/imgui/imgui)
include_directories(third_party/imgui/imgui/backends)

# glad
set(GLAD_SOURCES "third_party/glad/src/glad.c")
add_library("glad" ${GLAD_SOURCES})
include_directories(third_party/glad/include)

# glm
include_directories("${CMAKE_SOURCE_DIR}/third_party/glm")

# stb
include_directories("${CMAKE_SOURCE_DIR}/third_party/stb-master")

# Set CXX fLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Clang specific stuff
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-enum")
endif()

# Set CXX STANDARD
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_CONSOLE)

if(RESOURCE_INSTALL_DIR)
	add_definitions(-DROOT_DATA_DIR=\"${RESOURCE_INSTALL_DIR}/\")
	install(DIRECTORY data/ DESTINATION ${RESOURCE_INSTALL_DIR}/)
else()
	add_definitions(-DROOT_DATA_DIR=\"${CMAKE_SOURCE_DIR}/data/\")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

add_subdirectory(base)
add_subdirectory(examples)

