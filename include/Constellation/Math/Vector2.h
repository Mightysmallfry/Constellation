#ifndef CONSTELLATION_VECTOR2_H
#define CONSTELLATION_VECTOR2_H

#include <cstdint>

namespace constellation
{
    // Templated on scalar type — see Vector3.h for the rationale.
    template <typename T>
    class Vector2
    {
    public:
        T X = T(0);
        T Y = T(0);

        Vector2() = default;
        Vector2(T x, T y);

        Vector2 operator+(const Vector2& other) const;
        Vector2 operator-(const Vector2& other) const;
        Vector2 operator*(T scalar) const;
        Vector2& operator+=(const Vector2& other);
        Vector2& operator-=(const Vector2& other);

        T Length() const;
        T LengthSquared() const;
        Vector2 Normalized() const;
        void Normalize();

        static T Dot(const Vector2& a, const Vector2& b);
        static Vector2 Lerp(const Vector2& a, const Vector2& b, T t);

        static const Vector2 ZERO;
        static const Vector2 ONE;
    };

    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;
}

#endif