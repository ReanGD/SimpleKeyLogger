cmake_minimum_required(VERSION 3.10)
project(rtge VERSION 0.1 LANGUAGES CXX)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.14/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt  # or relative build/conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
set(STB_ERRORS_FILE "${CMAKE_SOURCE_DIR}/src/engine/material/image_loader.cpp")
set(IMGUI_SOURCE_FILES "${CONAN_SRC_DIRS_IMGUI}/bindings/imgui_impl_opengl3.cpp")

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
        -Werror

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
    )

    set(STB_ERRORS_EXCLUDE_FLAGS "-Wno-old-style-cast -Wno-sign-conversion -Wno-cast-qual -Wno-cast-align -Wno-implicit-int-conversion")
    set_source_files_properties(${STB_ERRORS_FILE} PROPERTIES COMPILE_FLAGS ${STB_ERRORS_EXCLUDE_FLAGS})

    set(IMGUI_ERRORS_EXCLUDE_FLAGS "-Wno-old-style-cast -Wno-sign-conversion")
    set_source_files_properties(${IMGUI_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${IMGUI_ERRORS_EXCLUDE_FLAGS})
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/libucl")
add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/libnoise")
add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/imgui_node_editor")

add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp" ${SOURCE_FILES} ${IMGUI_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "src" "${CONAN_SRC_DIRS_IMGUI}/bindings")
target_link_libraries(${PROJECT_NAME} PRIVATE ${CONAN_LIBS} ucl noise imgui_node_editor)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
