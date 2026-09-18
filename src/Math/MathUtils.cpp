#include "Math/MathUtils.h"

#include <cmath>

namespace constellation
{
    float MathUtils::Clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    float MathUtils::Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    float MathUtils::DegreesToRadians(float degrees)
    {
        return degrees * (Pi / 180.0f);
    }

    float MathUtils::RadiansToDegrees(float radians)
    {
        return radians * (180.0f / Pi);
    }

    bool MathUtils::NearlyEqual(float a, float b, float epsilon)
    {
        return std::fabs(a - b) <= epsilon;
    }
}
