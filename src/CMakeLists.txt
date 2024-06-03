set(SOURCES
    core/callback_storage.cpp
    core/engine.cpp
    core/game_window.cpp
    core/input_manager.cpp
    core/uuid_object.cpp
    core/editor/filesystem_tree_ui.cpp
    core/utils/game_time.cpp
    core/utils/logger.cpp
    game/game_object.cpp
    game/components/camera.cpp
    game/components/component.cpp
    game/components/cube.cpp
    game/components/drawable.cpp
    game/components/physical_body.cpp
    game/components/sprite.cpp
    game/components/test_comp_rotator.cpp
    game/components/transform.cpp
    game/scene/scene.cpp
    game/scene/scene_manager.cpp
    graphics/frame_buffer.cpp
    graphics/index_buffer.cpp
    graphics/piece.cpp
    graphics/renderer.cpp
    graphics/shader.cpp
    graphics/texture.cpp
    graphics/vertex_buffer.cpp
    io/resource_manager.cpp
    libs/imgui/imgui.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_impl_glfw.cpp
    libs/imgui/imgui_impl_opengl3.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    physics/bp_layer_interfacy.cpp
    physics/joltDebugRenderer.cpp
    physics/object_layer_pair_filter.cpp
    physics/object_vs_bp_layer_filter.cpp
    physics/physics_core.cpp
    scripts/script_api_3rd_party.cpp
    scripts/script_api_core.cpp
    scripts/script_api_game.cpp
    scripts/script_api_graphics.cpp
    scripts/script_api_scene.cpp
    scripts/script_core.cpp
)

add_library(CatEngine
    SHARED
    ${SOURCES}
)

add_compile_definitions(CatEngine CATENGINE_EXPORTS
    _WINDOWS
    WIN32
    _USRDLL
    JPH_PROFILE_ENABLED
    JPH_DEBUG_RENDERER
    JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
    JPH_USE_AVX2
    JPH_USE_AVX
    JPH_USE_SSE4_1
    JPH_USE_SSE4_2
    JPH_USE_LZCNT
    JPH_USE_TZCNT
    JPH_USE_F16C
    JPH_USE_FMADD
)

target_include_directories(CatEngine
    PUBLIC ${LIBS_GLEW_DIR}/include
    PUBLIC ${LIBS_GLFW_DIR}/include
    PUBLIC ${LIBS_GLM_DIR}
    PUBLIC ${LIBS_JOLT_DIR}
    PUBLIC ${LIBS_SOL2_DIR}/include
    PUBLIC ${LIBS_LUA_DIR} 
    PUBLIC ${CAT_SOURCES_DIR}
    PUBLIC ${LIBS_REFLECT_CPP_DIR}
    )
    
    
target_link_libraries(CatEngine 
    Jolt 
    lua 
    sol2 
    glfw
    glew
    reflectcpp
)

target_compile_definitions(CatEngine
    PUBLIC
    PUBLIC_SUM
    PRIVATE
    PRIVATE_SUM
)