set(TARGET 01_create_window)
add_executable(${TARGET} main.cxx runner.cxx runner.h)
target_include_directories(${TARGET} PUBLIC ${EXT_INC_DIR})
target_link_libraries(${TARGET} PUBLIC ${FMT_LIB} ${GLFW_LIB} glad2)