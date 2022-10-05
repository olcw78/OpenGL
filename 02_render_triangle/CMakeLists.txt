set(TARGET 02_render_triangle)
add_executable(${TARGET} main.cxx runner.cxx runner.h)
target_include_directories(${TARGET} PUBLIC ${EXT_INC_DIR} ${SHARED_INC_DIR})
target_link_libraries(${TARGET} PUBLIC ${FMT_LIB} ${GLFW_LIB} ${GLAD2_LIB} ${FSTREAM_GUARD_LIB})