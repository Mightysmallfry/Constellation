cmake_minimum_required(VERSION 3.20)
project(Constellation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CONSTELLATION_SOURCES
    src/Math/Vector2.cpp
    src/Math/Vector3.cpp
    src/Math/MathUtils.cpp
    src/Containers/Handle.cpp
)

add_library(Constellation STATIC ${CONSTELLATION_SOURCES})

target_include_directories(Constellation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(Constellation PUBLIC cxx_std_20)

# Keep warnings loud so portability issues surface early, on both compilers
# this is expected to build with (MSVC on Windows, GCC/Clang on Linux).
if (MSVC)
    target_compile_options(Constellation PRIVATE /W4)
else()
    target_compile_options(Constellation PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(CONSTELLATION_BUILD_TESTS "Build Constellation unit tests" OFF)
if (CONSTELLATION_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()