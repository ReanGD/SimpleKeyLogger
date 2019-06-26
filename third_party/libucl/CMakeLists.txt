cmake_minimum_required(VERSION 3.10)
project(ucl VERSION 0.1 LANGUAGES C)

file(GLOB_RECURSE UCL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

set(UCL_ERRORS_EXCLUDE_FLAGS "-Wno-sign-conversion -Wno-cast-qual -Wno-cast-align -Wno-pointer-sign -Wno-shorten-64-to-32 -Wno-unused-parameter -Wno-float-equal -Wno-float-conversion -Wno-implicit-int-conversion")
set_source_files_properties(${UCL_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${UCL_ERRORS_EXCLUDE_FLAGS})

add_library(${PROJECT_NAME} STATIC ${UCL_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
