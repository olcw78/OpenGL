set(TARGET texture)
add_library(${TARGET} STATIC
        texture.h
        texture.cxx)
target_include_directories(${TARGET} PRIVATE ${EXT_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${FMT_LIB} ${STB_IMAGE_LIB} ${GL_LIB})