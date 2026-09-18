#ifndef CONSTELLATION_VECTOR2_H
#define CONSTELLATION_VECTOR2_H

#include <cstdint>

namespace constellation
{
    class Vector2
    {
    public:
        float X = 0.0f;
        float Y = 0.0f;

        Vector2() = default;
        Vector2(float x, float y);

        Vector2 operator+(const Vector2& other) const;
        Vector2 operator-(const Vector2& other) const;
        Vector2 operator*(float scalar) const;
        Vector2& operator+=(const Vector2& other);
        Vector2& operator-=(const Vector2& other);

        float Length() const;
        float LengthSquared() const;
        Vector2 Normalized() const;
        void Normalize();

        static float Dot(const Vector2& a, const Vector2& b);
        static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

        static const Vector2 ZERO;
        static const Vector2 ONE;
    }; 
}

#endif