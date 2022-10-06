set(TARGET 02_render_triangle)
add_executable(${TARGET} main.cxx runner.cxx runner.h)
target_include_directories(${TARGET} PRIVATE ${EXT_INC_DIR} ${SHARED_INC_DIR} ${ASSET_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${GL_LIB} ${ASSET_LIB})