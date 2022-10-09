set(TARGET1 glad2)
add_library(${TARGET1} SHARED
        inc/glad/gl.h
        src/gl.cxx)

target_include_directories(${TARGET1} PRIVATE inc/)

set(TARGET2 stb_image)
add_library(${TARGET2} SHARED
        inc/stb/stb_image.h
        src/stb_image.cxx)

add_subdirectory(imgui)