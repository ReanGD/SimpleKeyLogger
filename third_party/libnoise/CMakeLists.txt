cmake_minimum_required(VERSION 3.10)
project(noise VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE LOC_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# set(LOC_COMPILE_FLAGS "-Wno-old-style-cast -Wno-float-equal -Wno-sign-conversion -Wno-bitwise-op-parentheses -Wno-float-conversion -O3")
set(LOC_COMPILE_FLAGS "-Wno-old-style-cast -Wno-float-equal -Wno-sign-conversion -Wno-bitwise-op-parentheses -Wno-float-conversion")
set_source_files_properties(${LOC_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${LOC_COMPILE_FLAGS})

add_library(${PROJECT_NAME} STATIC ${LOC_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
