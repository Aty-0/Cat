cmake_minimum_required(VERSION 3.29)

project(Cat)
# todo: 
option(CAT_BUILD_TESTS "Build the cat engine tests" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CPACK_PACKAGE_NAME "Cat")
set(CPACK_PACKAGE_VERSION "0.0.1")

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_MODULE_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_PREFIX_PATH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# TODO: Can we check version ?
set(LIBS_DIR_PATH "${PROJECT_SOURCE_DIR}/libs")
# Add a glfw lib 
set(LIBS_GLFW_DIR "${LIBS_DIR_PATH}/glfw")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${LIBS_GLFW_DIR})
# Add Jolt
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF) # We want to use a DLL 
set(LIBS_JOLT_DIR "${LIBS_DIR_PATH}/jolt")
add_subdirectory(${LIBS_JOLT_DIR}/build)
# Add glm
set(LIBS_GLM_DIR "${LIBS_DIR_PATH}/glm/")
add_subdirectory(${LIBS_GLM_DIR}/glm)
# Add sol2 and lua
set(LIBS_SOL2_DIR "${LIBS_DIR_PATH}/sol2")
add_subdirectory(${LIBS_SOL2_DIR})

set(LIBS_LUA_DIR "${LIBS_DIR_PATH}/lua-5.4.6/src")
add_library(
    lua STATIC
    ${LIBS_LUA_DIR}/lapi.c
    ${LIBS_LUA_DIR}/lauxlib.c
    ${LIBS_LUA_DIR}/lbaselib.c
    ${LIBS_LUA_DIR}/lcode.c
    ${LIBS_LUA_DIR}/lcorolib.c
    ${LIBS_LUA_DIR}/lctype.c
    ${LIBS_LUA_DIR}/ldblib.c
    ${LIBS_LUA_DIR}/ldebug.c
    ${LIBS_LUA_DIR}/ldo.c
    ${LIBS_LUA_DIR}/ldump.c
    ${LIBS_LUA_DIR}/lfunc.c
    ${LIBS_LUA_DIR}/lgc.c
    ${LIBS_LUA_DIR}/linit.c
    ${LIBS_LUA_DIR}/liolib.c
    ${LIBS_LUA_DIR}/llex.c
    ${LIBS_LUA_DIR}/lmathlib.c
    ${LIBS_LUA_DIR}/lmem.c
    ${LIBS_LUA_DIR}/loadlib.c
    ${LIBS_LUA_DIR}/lobject.c
    ${LIBS_LUA_DIR}/lopcodes.c
    ${LIBS_LUA_DIR}/loslib.c
    ${LIBS_LUA_DIR}/lparser.c
    ${LIBS_LUA_DIR}/lstate.c
    ${LIBS_LUA_DIR}/lstring.c
    ${LIBS_LUA_DIR}/lstrlib.c
    ${LIBS_LUA_DIR}/ltable.c
    ${LIBS_LUA_DIR}/ltablib.c
    ${LIBS_LUA_DIR}/ltm.c
    ${LIBS_LUA_DIR}/lua.c
    ${LIBS_LUA_DIR}/lundump.c
    ${LIBS_LUA_DIR}/lutf8lib.c
    ${LIBS_LUA_DIR}/lvm.c
    ${LIBS_LUA_DIR}/lzio.c
)

# Add OpenGL and GLEW
find_package(OpenGL REQUIRED)
set(LIBS_GLEW_DIR "${PROJECT_SOURCE_DIR}/libs/glew")
add_subdirectory("${LIBS_GLEW_DIR}/build/cmake")
#add_library(glew STATIC ${LIBS_GLEW_DIR}/src/glew.c)
target_include_directories(glew PUBLIC ${LIBS_GLEW_DIR}/include)
#target_link_libraries(glew OpenGL::GL)

set(CAT_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")
message(${CAT_SOURCES_DIR})
add_subdirectory(application)
add_subdirectory(src)

include(CPack)