cmake_minimum_required(VERSION 3.0.0)
project(rtge VERSION 0.1.0)

find_package(glfw3 3.3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

include_directories("src")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_executable(${PROJECT_NAME} "src/main.cpp" ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_LIBRARY} ${GLEW_LIBRARIES})
