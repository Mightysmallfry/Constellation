#include "Math/Vector3.h"
#include "Math/MathUtils.h"

#include <cmath>
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

namespace
{
    // MathUtils::NearlyEqual is float-only for now, so run the same
    // checks generically across both Vector3 precisions with a
    // type-appropriate epsilon.
    template <typename T>
    bool NearlyEqualT(T a, T b, T epsilon = T(1e-5))
    {
        return std::fabs(a - b) <= epsilon;
    }

    template <typename T>
    void RunVector3Checks()
    {
        using constellation::Vector3;

        Vector3<T> a(T(1), T(2), T(3));
        Vector3<T> b(T(4), T(5), T(6));

        Vector3<T> sum = a + b;
        CHECK(NearlyEqualT<T>(sum.X, T(5)));
        CHECK(NearlyEqualT<T>(sum.Y, T(7)));
        CHECK(NearlyEqualT<T>(sum.Z, T(9)));

        CHECK(NearlyEqualT<T>(Vector3<T>::Dot(a, b), T(32)));

        Vector3<T> cross = Vector3<T>::Cross(Vector3<T>::UNIT_X, Vector3<T>::UNIT_Y);
        CHECK(NearlyEqualT<T>(cross.X, T(0)));
        CHECK(NearlyEqualT<T>(cross.Y, T(0)));
        CHECK(NearlyEqualT<T>(cross.Z, T(1)));

        Vector3<T> normalized = Vector3<T>(T(3), T(0), T(0)).Normalized();
        CHECK(NearlyEqualT<T>(normalized.Length(), T(1)));
    }
}

int main()
{
    using constellation::MathUtils;

    RunVector3Checks<float>();
    std::printf("Vector3<float> tests passed.\n");

    RunVector3Checks<double>();
    std::printf("Vector3<double> tests passed.\n");

    // MathUtils itself is still float-only.
    CHECK(MathUtils::NearlyEqual(1.0f, 1.0000001f));

    std::printf("All tests passed.\n");
    return 0;
}