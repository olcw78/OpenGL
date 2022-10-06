set(EXT_LIB_DIR "${PROJECT_SOURCE_DIR}/external/lib")

set(EXT_INC_DIR "${PROJECT_SOURCE_DIR}/external/inc")
set(SHARED_INC_DIR "${PROJECT_SOURCE_DIR}/shared")
set(ASSET_INC_DIR "${PROJECT_SOURCE_DIR}/assets")
set(GLM_INC_DIR "${CMAKE_INSTALL_PREFIX}/include")

# lib
set(FMT_LIB "${EXT_LIB_DIR}/fmt/libfmt.9.1.0.dylib")
set(GLFW_LIB "${EXT_LIB_DIR}/GLFW/libglfw.3.dylib")
set(GLAD2_LIB glad2)
set(STB_IMAGE_LIB stb_image)
set(FSTREAM_GUARD_LIB fstream_guard)
set(SHADER_LIB shader opengl_status_checker)
set(ASSET_LIB assets)

# lib collection
set(GL_LIB
        ${GLFW_LIB}
        ${GLAD2_LIB})

set(UTIL_LIBS
        ${FMT_LIB}
        ${FSTREAM_GUARD_LIB}
        ${SHADER_LIB}
        ${ASSET_LIB})
