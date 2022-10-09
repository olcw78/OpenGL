# root

cmake_minimum_required(VERSION 3.23)
project(opengl)

set(CMAKE_CXX_STANDARD 17
        VERSION 0.1.0
        LANGUAGES CXX)

include(VAR.cmake)

# shareds
add_subdirectory(external)
add_subdirectory(shared)
add_subdirectory(assets)

# projects
#add_subdirectory(01_basic)
add_subdirectory(02_lighting)