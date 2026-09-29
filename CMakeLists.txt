cmake_minimum_required(VERSION 3.21)
project(Constellation VERSION 0.1.0 LANGUAGES CXX)

option(CONSTELLATION_BUILD_TESTS "Build Constellation unit tests" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(Constellation STATIC
    src/Containers/Handle.cpp
    src/Math/MathUtils.cpp
    src/Math/Vector2.cpp
    src/Math/Vector3.cpp
)
add_library(Constellation::Constellation ALIAS Constellation)

target_include_directories(Constellation
    PUBLIC
        # Consumers (and Constellation itself): #include <Constellation/Math/Vector2.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        # Constellation's own sources only: #include "Math/Vector2.h"
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Constellation
)

if(MSVC)
    target_compile_options(Constellation PRIVATE /W4)
else()
    target_compile_options(Constellation PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CONSTELLATION_BUILD_TESTS)
    enable_testing()
    find_package(Catch2 3 CONFIG REQUIRED)

    add_executable(ConstellationTests
        tests/Math/Vector3Tests.cpp
    )
    target_include_directories(ConstellationTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Constellation
    )
    target_link_libraries(ConstellationTests PRIVATE
        Constellation
        Catch2::Catch2WithMain
    )

    include(Catch)
    catch_discover_tests(ConstellationTests)
endif()
