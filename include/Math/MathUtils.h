#ifndef CONSTELLATION_MATHUTIL_H
#define CONSTELLATION_MATHUTIL_H

namespace constellation
{
    class MathUtils
    {
    public:
        static constexpr float Pi = 3.14159265358979323846f;

        static float Clamp(float value, float min, float max);
        static float Lerp(float a, float b, float t);
        static float DegreesToRadians(float degrees);
        static float RadiansToDegrees(float radians);
        static bool NearlyEqual(float a, float b, float epsilon = 1e-5f);
    };
}

#endif