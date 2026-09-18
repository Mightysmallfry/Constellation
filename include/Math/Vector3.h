#ifndef CONSTELLATION_VECTOR3_H
#define CONSTELLATION_VECTOR3_H

namespace constellation
{
    class Vector3
    {
    public:
        float X = 0.0f;
        float Y = 0.0f;
        float Z = 0.0f;

        Vector3() = default;
        Vector3(float x, float y, float z);

        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator-() const;
        Vector3 operator*(float scalar) const;
        Vector3& operator+=(const Vector3& other);
        Vector3& operator-=(const Vector3& other);
        Vector3& operator*=(float scalar);

        float Length() const;
        float LengthSquared() const;
        Vector3 Normalized() const;
        void Normalize();

        static float Dot(const Vector3& a, const Vector3& b);
        static Vector3 Cross(const Vector3& a, const Vector3& b);
        static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

        static const Vector3 ZERO;
        static const Vector3 ONE;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
    };
}

#endif