#include "Math/Vector2.h"

#include <cmath>

namespace constellation
{
    template <typename T> const Vector2<T> Vector2<T>::ZERO = Vector2<T>(T(0), T(0));
    template <typename T> const Vector2<T> Vector2<T>::ONE = Vector2<T>(T(1), T(1));

    template <typename T>
    Vector2<T>::Vector2(T x, T y)
        : X(x), Y(y)
    {
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
    {
        return Vector2<T>(X + other.X, Y + other.Y);
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const
    {
        return Vector2<T>(X - other.X, Y - other.Y);
    }

    template <typename T>
    Vector2<T> Vector2<T>::operator*(T scalar) const
    {
        return Vector2<T>(X * scalar, Y * scalar);
    }

    template <typename T>
    Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
    {
        X += other.X;
        Y += other.Y;
        return *this;
    }

    template <typename T>
    Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
    {
        X -= other.X;
        Y -= other.Y;
        return *this;
    }

    template <typename T>
    T Vector2<T>::LengthSquared() const
    {
        return X * X + Y * Y;
    }

    template <typename T>
    T Vector2<T>::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    template <typename T>
    Vector2<T> Vector2<T>::Normalized() const
    {
        const T len = Length();
        if (len <= T(0))
        {
            return Vector2<T>::ZERO;
        }
        return Vector2<T>(X / len, Y / len);
    }

    template <typename T>
    void Vector2<T>::Normalize()
    {
        *this = Normalized();
    }

    template <typename T>
    T Vector2<T>::Dot(const Vector2<T>& a, const Vector2<T>& b)
    {
        return a.X * b.X + a.Y * b.Y;
    }

    template <typename T>
    Vector2<T> Vector2<T>::Lerp(const Vector2<T>& a, const Vector2<T>& b, T t)
    {
        return a + (b - a) * t;
    }

    template class Vector2<float>;
    template class Vector2<double>;
}