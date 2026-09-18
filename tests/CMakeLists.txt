add_executable(ConstellationTests
    Math/Vector3Tests.cpp
)

target_link_libraries(ConstellationTests PRIVATE Constellation)

add_test(NAME ConstellationTests COMMAND ConstellationTests)