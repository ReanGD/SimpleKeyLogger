cmake_minimum_required(VERSION 3.10)
project(rtge VERSION 0.1 LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD_TYPE "Release"
                BUILD missing)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(ENGINE_DIR "${CMAKE_SOURCE_DIR}/src/engine")

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
set(STB_ERROR_SOURCE_FILES "${ENGINE_DIR}/material/image_loader.cpp")
set(NOISE_ERROR_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/middleware/node_editor/noiseutils.cpp")
set(PHYSICS_ERROR_SOURCE_FILES "${ENGINE_DIR}/physics/physics.cpp")
set(PHYSICS2_ERROR_SOURCE_FILES "${ENGINE_DIR}/physics/physical_node.cpp")
set(IMGUI_ERROR_SOURCE_FILES "${CONAN_SRC_DIRS_IMGUI}/bindings/imgui_impl_opengl3.cpp")

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

    -Wno-nested-anon-types
    -Wno-gnu-anonymous-struct
)

set(STB_ERROR_COMPILE_FLAGS "-Wno-old-style-cast -Wno-sign-conversion -Wno-cast-qual -Wno-cast-align -Wno-implicit-int-conversion -Wno-unused-parameter")
set_source_files_properties(${STB_ERROR_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${STB_ERROR_COMPILE_FLAGS})

set(NOISE_ERROR_COMPILE_FLAGS "-Wno-old-style-cast -Wno-float-equal")
set_source_files_properties(${NOISE_ERROR_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${NOISE_ERROR_COMPILE_FLAGS})

set(PHYSICS_ERROR_COMPILE_FLAGS "-Wno-old-style-cast -Wno-sign-conversion -Wno-float-equal -Wno-implicit-float-conversion -Wno-unused-parameter -Wno-cast-align")
set_source_files_properties(${PHYSICS_ERROR_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${PHYSICS_ERROR_COMPILE_FLAGS})

set(PHYSICS2_ERROR_COMPILE_FLAGS "-Wno-old-style-cast -Wno-sign-conversion -Wno-float-equal -Wno-implicit-float-conversion ")
set_source_files_properties(${PHYSICS2_ERROR_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${PHYSICS2_ERROR_COMPILE_FLAGS})

set(IMGUI_ERROR_COMPILE_FLAGS "-Wno-old-style-cast -Wno-sign-conversion")
set_source_files_properties(${IMGUI_ERROR_SOURCE_FILES} PROPERTIES COMPILE_FLAGS ${IMGUI_ERROR_COMPILE_FLAGS})

add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/libucl")
add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/imgui_node_editor")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${IMGUI_ERROR_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "src" "${CONAN_SRC_DIRS_IMGUI}/bindings")
target_link_libraries(${PROJECT_NAME} PRIVATE ${CONAN_LIBS} ucl imgui_node_editor)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
