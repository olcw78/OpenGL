set(TARGET assets)
add_library(${TARGET} STATIC
        texture/texture_assets.h
        texture/texture_assets.cxx
        shader/e_shader_asset_type.h
        shader/shader_assets.h
        shader/shader_assets.cxx
        assets_inc.h)
target_include_directories(${TARGET} PRIVATE ${EXT_INC_DIR})
target_link_libraries(${TARGET} PRIVATE ${FMT_LIB})