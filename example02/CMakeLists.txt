cmake_minimum_required(VERSION 3.17)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

include(../cmake/CommonMacros.txt)

include_directories(../third_party/glfw/include)
include_directories(../third_party/glad/include)
include_directories(../third_party/glm)

include_directories("include/")

file(GLOB_RECURSE HEADER_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/*.h")
file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
file(GLOB SHADERS_FILES "shader/*.glsl")

add_executable(${PROJECT_NAME}  ${SRC_FILES} ${HEADER_FILES})

if(MSVC)
	add_definitions(-D_CONSOLE)
endif()

add_definitions(-DSHADERFOLDER="${CMAKE_CURRENT_SOURCE_DIR}/shader")

SETUP_GROUPS("${SRC_FILES}")
SETUP_GROUPS("${HEADER_FILES}")
SETUP_GROUPS("${SHADERS_FILES}")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw imgui)

