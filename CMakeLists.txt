cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(node-glslang LANGUAGES CXX)
cmake_policy(SET CMP0025 NEW)
set(CMAKE_CXX_STANDARD 20)

set(ENABLE_OPT OFF CACHE BOOL "")

add_subdirectory("vendor/glslang" glslang)

add_library(
    node-glslang
    SHARED
        src/module.cpp
        src/glsl_validate.cpp
        src/glsl_validate.hpp
)

target_include_directories(
    node-glslang
    PRIVATE
        ${PROJECT_SOURCE_DIR}/vendor/node-api-headers/include
        ${PROJECT_SOURCE_DIR}/vendor/node-addon-api
)

target_link_libraries(
    node-glslang
    PRIVATE
        glslang
)

target_compile_definitions(
    node-glslang
    PRIVATE
        NAPI_VERSION=8
        NODE_ADDON_API_DISABLE_DEPRECATED
)

set_target_properties(
    node-glslang
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "module"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/binding/$<0:>"
        SUFFIX ".node"
)

if (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    set_target_properties(
        node-glslang
        PROPERTIES
            LINK_DEPENDS ${PROJECT_SOURCE_DIR}/cmake/symbols.txt
            LINK_FLAGS "-undefined dynamic_lookup -bind_at_load"
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set_target_properties(
        node-glslang
        PROPERTIES
            LINK_DEPENDS ${PROJECT_SOURCE_DIR}/cmake/symbols.def
    )
else()
    set_target_properties(
        node-glslang
        PROPERTIES
            LINK_DEPENDS ${PROJECT_SOURCE_DIR}/cmake/symbols.ld
            LINK_FLAGS "-z now"
    )
endif()
