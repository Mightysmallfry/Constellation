#include "Math/Vector3.h"
#include "Math/MathUtils.h"

#include <cstdio>
#include <cstdlib>

// Minimal dependency-free smoke tests. Swap in a real framework
// (Catch2, doctest, GTest) later if the test suite grows.
#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            std::fprintf(stderr, "FAILED: %s (line %d)\n", #condition, __LINE__); \
            std::exit(1); \
        } \
    } while (0)

int main()
{
    using constellation::Vector3;
    using constellation::MathUtils;

    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);

    Vector3 sum = a + b;
    CHECK(MathUtils::NearlyEqual(sum.X, 5.0f));
    CHECK(MathUtils::NearlyEqual(sum.Y, 7.0f));
    CHECK(MathUtils::NearlyEqual(sum.Z, 9.0f));

    CHECK(MathUtils::NearlyEqual(Vector3::Dot(a, b), 32.0f));

    Vector3 cross = Vector3::Cross(Vector3::UNIT_X, Vector3::UNIT_Y);
    CHECK(MathUtils::NearlyEqual(cross.X, 0.0f));
    CHECK(MathUtils::NearlyEqual(cross.Y, 0.0f));
    CHECK(MathUtils::NearlyEqual(cross.Z, 1.0f));

    Vector3 normalized = Vector3(3.0f, 0.0f, 0.0f).Normalized();
    CHECK(MathUtils::NearlyEqual(normalized.Length(), 1.0f));

    std::printf("All Vector3 tests passed.\n");
    return 0;
}
