#ifndef CONSTELLATION_VECTOR3_H
#define CONSTELLATION_VECTOR3_H

namespace constellation
{
    // Templated on scalar type so float and double vectors can coexist
    // in the same program (e.g. float for rendering, double for a
    // scientific-accuracy integrator). See Vector3.cpp for the explicit
    // instantiations this ships with.
    template <typename T>
    class Vector3
    {
    public:
        T X = T(0);
        T Y = T(0);
        T Z = T(0);

        Vector3() = default;
        Vector3(T x, T y, T z);

        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator-() const;
        Vector3 operator*(T scalar) const;
        Vector3& operator+=(const Vector3& other);
        Vector3& operator-=(const Vector3& other);
        Vector3& operator*=(T scalar);

        T Length() const;
        T LengthSquared() const;
        Vector3 Normalized() const;
        void Normalize();

        static T Dot(const Vector3& a, const Vector3& b);
        static Vector3 Cross(const Vector3& a, const Vector3& b);
        static Vector3 Lerp(const Vector3& a, const Vector3& b, T t);

        static const Vector3 ZERO;
        static const Vector3 ONE;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
    };

    // Convenience aliases. There is no bare, un-templated "Vector3" —
    // callers pick a precision explicitly.
    using Vector3f = Vector3<float>;
    using Vector3d = Vector3<double>;
}

#endif