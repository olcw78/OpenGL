set(TARGET opengl_status_checker)
add_library(${TARGET} STATIC
        opengl_status_checker.h
        opengl_status_checker.cxx)
target_include_directories(${TARGET} PRIVATE ${EXT_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${FMT_LIB} ${GLFW_LIB} ${GLAD2_LIB})