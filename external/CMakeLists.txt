set(TARGET glad2)
add_library(${TARGET} SHARED
        inc/glad/gl.h
        src/gl.cxx)

target_include_directories(${TARGET} PRIVATE inc/)