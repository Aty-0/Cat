add_executable(Cat main.cpp)

target_include_directories(Cat
    PUBLIC ${LIBS_GLEW_DIR}/include
    PUBLIC ${LIBS_GLFW_DIR}/include
    PUBLIC ${LIBS_GLM_DIR}
    PUBLIC ${LIBS_JOLT_DIR}
    PUBLIC ${LIBS_SOL2_DIR}/include
    PUBLIC ${LIBS_LUA_DIR} 
    PUBLIC ${CAT_SOURCES_DIR}
    )
target_compile_features(Cat PRIVATE cxx_std_20)
   
target_link_libraries(Cat
    CatEngine
    Jolt 
    lua 
    sol2 
    glfw
    glew
)
