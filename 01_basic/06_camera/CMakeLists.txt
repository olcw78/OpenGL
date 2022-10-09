find_package(glm)

set(TARGET 06_camera)
add_executable(${TARGET} main.cxx runner.h runner.cxx)
target_include_directories(${TARGET} PRIVATE ${EXT_INC_DIR} ${SHARED_INC_DIR} ${ASSET_INC_DIR} ${GLM_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${GL_LIB} ${SHADER_LIB} ${STB_IMAGE_LIB} ${UTIL_LIBS})