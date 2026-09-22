cmake_minimum_required(VERSION 3.20)
project(Constellation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CONSTELLATION_SOURCES
    src/Math/Vector2.cpp
    src/Math/Vector3.cpp
    src/Math/MathUtils.cpp
    src/Containers/Handle.cpp
    src/Parsers/StepFileParser.cpp
)

add_library(Constellation STATIC ${CONSTELLATION_SOURCES})

target_include_directories(Constellation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Constellation
)

target_compile_features(Constellation PUBLIC cxx_std_20)

# OpenCASCADE is only needed to *build* StepFileParser.cpp. StepFileParser.h
# forward-declares the OCCT types it uses (Bnd_Box, TopoDS_Solid) instead of
# including OCCT headers, so nothing in Constellation's public interface
# exposes OCCT. PRIVATE keeps OCCT's include dirs off of every consumer of
# Constellation (e.g. Aquarius) -- they only ever see plain std types and
# Constellation's own headers.
find_package(OpenCASCADE CONFIG REQUIRED)

target_link_libraries(Constellation PRIVATE
    TKDESTEP     # STEP reading
    TKernel    # core OCCT types
    TKBRep     # B-rep topology (TopoDS_*)
    TKMesh     # BRepMesh_IncrementalMesh, Poly_Triangulation
    TKTopAlgo  # TopExp_Explorer and related topology algorithms
    TKMath     # gp_Pnt, gp_Trsf, gp_Vec
)

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