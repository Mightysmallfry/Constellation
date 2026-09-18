#include "Math/Vector3.h"

#include <cmath>

namespace constellation
{
    template <typename T> const Vector3<T> Vector3<T>::ZERO = Vector3<T>(T(0), T(0), T(0));
    template <typename T> const Vector3<T> Vector3<T>::ONE = Vector3<T>(T(1), T(1), T(1));
    template <typename T> const Vector3<T> Vector3<T>::UNIT_X = Vector3<T>(T(1), T(0), T(0));
    template <typename T> const Vector3<T> Vector3<T>::UNIT_Y = Vector3<T>(T(0), T(1), T(0));
    template <typename T> const Vector3<T> Vector3<T>::UNIT_Z = Vector3<T>(T(0), T(0), T(1));

    template <typename T>
    Vector3<T>::Vector3(T x, T y, T z)
        : X(x), Y(y), Z(z)
    {
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const
    {
        return Vector3<T>(X + other.X, Y + other.Y, Z + other.Z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator-(const Vector3<T>& other) const
    {
        return Vector3<T>(X - other.X, Y - other.Y, Z - other.Z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator-() const
    {
        return Vector3<T>(-X, -Y, -Z);
    }

    template <typename T>
    Vector3<T> Vector3<T>::operator*(T scalar) const
    {
        return Vector3<T>(X * scalar, Y * scalar, Z * scalar);
    }

    template <typename T>
    Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        return *this;
    }

    template <typename T>
    Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& other)
    {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
        return *this;
    }

    template <typename T>
    Vector3<T>& Vector3<T>::operator*=(T scalar)
    {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;
        return *this;
    }

    template <typename T>
    T Vector3<T>::LengthSquared() const
    {
        return X * X + Y * Y + Z * Z;
    }

    template <typename T>
    T Vector3<T>::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    template <typename T>
    Vector3<T> Vector3<T>::Normalized() const
    {
        const T len = Length();
        if (len <= T(0))
        {
            return Vector3<T>::ZERO;
        }
        return Vector3<T>(X / len, Y / len, Z / len);
    }

    template <typename T>
    void Vector3<T>::Normalize()
    {
        *this = Normalized();
    }

    template <typename T>
    T Vector3<T>::Dot(const Vector3<T>& a, const Vector3<T>& b)
    {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }

    template <typename T>
    Vector3<T> Vector3<T>::Cross(const Vector3<T>& a, const Vector3<T>& b)
    {
        return Vector3<T>(
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        );
    }

    template <typename T>
    Vector3<T> Vector3<T>::Lerp(const Vector3<T>& a, const Vector3<T>& b, T t)
    {
        return a + (b - a) * t;
    }

    // Explicit instantiation: these are the only precisions this library
    // ships. Adding a new one (e.g. long double) means adding a line here.
    template class Vector3<float>;
    template class Vector3<double>;
}