cmake_minimum_required(VERSION 3.23)
project(opengl)

set(CMAKE_CXX_STANDARD 17
        VERSION 0.1.0
        LANGUAGES CXX)

set(EXT_INC_DIR "${PROJECT_SOURCE_DIR}/external/inc")
set(EXT_LIB_DIR "${PROJECT_SOURCE_DIR}/external/lib")

# lib
set(FMT_LIB "${EXT_LIB_DIR}/fmt/libfmt.9.1.0.dylib")
set(GLFW_LIB "${EXT_LIB_DIR}/GLFW/libglfw.3.dylib")

# lib names
set(GLAD2_LIB glad2)

add_subdirectory(external)
add_subdirectory(01_create_window)