set(TARGET 03_shaders)
add_executable(${TARGET} main.cxx runner.cxx runner.h)
target_include_directories(${TARGET} PRIVATE ${EXT_INC_DIR} ${SHARED_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${FMT_LIB} ${GLFW_LIB} ${GLAD2_LIB} ${SHADER_LIB})