#include "Math/Vector2.h"

#include <cmath>

namespace constellation
{
    const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
    const Vector2 Vector2::ONE = Vector2(1.0f, 1.0f);

    Vector2::Vector2(float x, float y)
        : X(x), Y(y)
    {
    }

    Vector2 Vector2::operator+(const Vector2& other) const
    {
        return Vector2(X + other.X, Y + other.Y);
    }

    Vector2 Vector2::operator-(const Vector2& other) const
    {
        return Vector2(X - other.X, Y - other.Y);
    }

    Vector2 Vector2::operator*(float scalar) const
    {
        return Vector2(X * scalar, Y * scalar);
    }

    Vector2& Vector2::operator+=(const Vector2& other)
    {
        X += other.X;
        Y += other.Y;
        return *this;
    }

    Vector2& Vector2::operator-=(const Vector2& other)
    {
        X -= other.X;
        Y -= other.Y;
        return *this;
    }

    float Vector2::LengthSquared() const
    {
        return X * X + Y * Y;
    }

    float Vector2::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    Vector2 Vector2::Normalized() const
    {
        const float len = Length();
        if (len <= 0.0f)
        {
            return Vector2::ZERO;
        }
        return Vector2(X / len, Y / len);
    }

    void Vector2::Normalize()
    {
        *this = Normalized();
    }

    float Vector2::Dot(const Vector2& a, const Vector2& b)
    {
        return a.X * b.X + a.Y * b.Y;
    }

    Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
    {
        return a + (b - a) * t;
    }
}
