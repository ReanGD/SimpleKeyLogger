cmake_minimum_required(VERSION 3.10)
project(imgui_node_editor VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE IMGUI_NODE_EDITOR_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(IMGUI_NODE_EDITOR_COMPILE_FLAGS "-Wno-float-equal -Wno-float-conversion -Wno-old-style-cast -Wno-sign-conversion -Wno-shorten-64-to-32 -Wno-unused-parameter")
set_source_files_properties(${IMGUI_NODE_EDITOR_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${IMGUI_NODE_EDITOR_COMPILE_FLAGS})

add_library(${PROJECT_NAME} STATIC ${IMGUI_NODE_EDITOR_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
