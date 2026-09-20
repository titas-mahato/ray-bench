cmake_minimum_required(VERSION 3.14)
project(ray-bench VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization flags for Release builds
if (MSVC)
    add_compile_options(/O2 /Oi /Ot /GL /W3 /MP)
else()
    add_compile_options(-O3 -march=native -Wall -Wextra)
endif()

# Fetch Raylib 5.0 directly
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
    GIT_SHALLOW TRUE
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# Source files
set(SOURCES
    src/main.cpp
)

add_executable(ray-bench ${SOURCES})

target_include_directories(ray-bench PRIVATE src)
target_link_libraries(ray-bench PRIVATE raylib)

# Output directory configuration
set_target_properties(ray-bench PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
)
