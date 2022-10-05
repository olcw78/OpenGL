cmake_minimum_required(VERSION 3.23)
project(opengl)

set(CMAKE_CXX_STANDARD 17
        VERSION 0.1.0
        LANGUAGES CXX)

set(EXT_INC_DIR "${PROJECT_SOURCE_DIR}/external/inc")
set(EXT_LIB_DIR "${PROJECT_SOURCE_DIR}/external/lib")
set(SHARED_INC_DIR "${PROJECT_SOURCE_DIR}/shared")

# lib
set(FMT_LIB "${EXT_LIB_DIR}/fmt/libfmt.9.1.0.dylib")
set(GLFW_LIB "${EXT_LIB_DIR}/GLFW/libglfw.3.dylib")
set(GLAD2_LIB glad2)
set(FSTREAM_GUARD_LIB fstream_guard)

add_subdirectory(external)
add_subdirectory(shared)
add_subdirectory(01_create_window)
add_subdirectory(02_render_triangle)