find_package(Catch2 3 CONFIG REQUIRED)

add_executable(ConstellationTests
    Math/Vector3Tests.cpp
    Parsing/G22Tests.cpp
)

target_link_libraries(ConstellationTests PRIVATE
    Constellation
    Catch2::Catch2WithMain
)
target_include_directories(ConstellationTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(NAME ConstellationTests COMMAND ConstellationTests)