cmake_minimum_required(VERSION 3.23)
project(opengl)

set(CMAKE_CXX_STANDARD 17
        VERSION 0.1.0
        LANGUAGES CXX)

include(VAR.cmake)

add_subdirectory(external)
add_subdirectory(shared)
add_subdirectory(assets)
add_subdirectory(01_create_window)
add_subdirectory(02_render_triangle)
add_subdirectory(03_shaders)
add_subdirectory(04_textures)
add_subdirectory(05_transformations)
add_subdirectory(06_camera)
add_subdirectory(07_colors)
add_subdirectory(08_basic_lighting)