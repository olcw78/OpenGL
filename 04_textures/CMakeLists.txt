set(TARGET 04_textures)
add_executable(${TARGET} main.cxx runner.cxx runner.h)
target_include_directories(${TARGET} PRIVATE ${EXT_INC_DIR} ${SHARED_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${FMT_LIB} ${GLFW_LIB} ${GLAD2_LIB} ${SHADER_LIB} ${STB_IMAGE_LIB})