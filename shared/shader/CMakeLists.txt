set(TARGET shader)
add_library(${TARGET} STATIC
        shader.h
        shader.cxx e_shader_type.h)
target_include_directories(${TARGET} PRIVATE ${EXT_INC_DIR} ${GLM_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${FMT_LIB} ${GL_LIB})