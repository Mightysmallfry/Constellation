#include "Math/Vector3.h"

#include <cmath>

namespace constellation
{
    const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::ONE = Vector3(1.0f, 1.0f, 1.0f);
    const Vector3 Vector3::UNIT_X = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::UNIT_Y = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::UNIT_Z = Vector3(0.0f, 0.0f, 1.0f);

    Vector3::Vector3(float x, float y, float z)
        : X(x), Y(y), Z(z)
    {
    }

    Vector3 Vector3::operator+(const Vector3& other) const
    {
        return Vector3(X + other.X, Y + other.Y, Z + other.Z);
    }

    Vector3 Vector3::operator-(const Vector3& other) const
    {
        return Vector3(X - other.X, Y - other.Y, Z - other.Z);
    }

    Vector3 Vector3::operator-() const
    {
        return Vector3(-X, -Y, -Z);
    }

    Vector3 Vector3::operator*(float scalar) const
    {
        return Vector3(X * scalar, Y * scalar, Z * scalar);
    }

    Vector3& Vector3::operator+=(const Vector3& other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& other)
    {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
        return *this;
    }

    Vector3& Vector3::operator*=(float scalar)
    {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;
        return *this;
    }

    float Vector3::LengthSquared() const
    {
        return X * X + Y * Y + Z * Z;
    }

    float Vector3::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    Vector3 Vector3::Normalized() const
    {
        const float len = Length();
        if (len <= 0.0f)
        {
            return Vector3::ZERO;
        }
        return Vector3(X / len, Y / len, Z / len);
    }

    void Vector3::Normalize()
    {
        *this = Normalized();
    }

    float Vector3::Dot(const Vector3& a, const Vector3& b)
    {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }

    Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
    {
        return Vector3(
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        );
    }

    Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
    {
        return a + (b - a) * t;
    }
}
