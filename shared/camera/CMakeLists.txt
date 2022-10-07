set(TARGET camera)
add_library(${TARGET} STATIC
        camera.h
        camera.cxx)
target_include_directories(${TARGET} PRIVATE ${GLM_INC_DIR} ${EXT_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${GL_LIB})