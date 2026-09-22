#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Math/MathUtils.h"
#include "Math/Vector3.h"

#include <cmath>

using constellation::MathUtils;
using constellation::Vector3;

namespace
{
    // MathUtils::NearlyEqual is float-only for now, so run the same checks
    // generically across both Vector3 precisions with a type-appropriate epsilon.
    template <typename T>
    bool NearlyEqualT(T a, T b, T epsilon = T(1e-5))
    {
        return std::fabs(a - b) <= epsilon;
    }
}

TEMPLATE_TEST_CASE("Vector3 arithmetic and geometry", "[Math][Vector3]", float, double)
{
    using T = TestType;

    Vector3<T> a(T(1), T(2), T(3));
    Vector3<T> b(T(4), T(5), T(6));

    SECTION("addition")
    {
        Vector3<T> sum = a + b;
        CHECK(NearlyEqualT<T>(sum.X, T(5)));
        CHECK(NearlyEqualT<T>(sum.Y, T(7)));
        CHECK(NearlyEqualT<T>(sum.Z, T(9)));
    }

    SECTION("dot product")
    {
        CHECK(NearlyEqualT<T>(Vector3<T>::Dot(a, b), T(32)));
    }

    SECTION("cross product of unit axes")
    {
        Vector3<T> cross = Vector3<T>::Cross(Vector3<T>::UNIT_X, Vector3<T>::UNIT_Y);
        CHECK(NearlyEqualT<T>(cross.X, T(0)));
        CHECK(NearlyEqualT<T>(cross.Y, T(0)));
        CHECK(NearlyEqualT<T>(cross.Z, T(1)));
    }

    SECTION("normalization")
    {
        Vector3<T> normalized = Vector3<T>(T(3), T(0), T(0)).Normalized();
        CHECK(NearlyEqualT<T>(normalized.Length(), T(1)));
    }
}

TEST_CASE("MathUtils::NearlyEqual tolerates float rounding", "[Math][MathUtils]")
{
    CHECK(MathUtils::NearlyEqual(1.0f, 1.0000001f));
}