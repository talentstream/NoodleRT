//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include <cassert>
#include <cmath>
#include <algorithm>
#include <stacktrace>
#include <iostream>

NAMESPACE_BEGIN

template<typename T>
class Vector2;

template<typename T>
class Vector3;

template<typename T>
class Point2;

template<typename T>
class Point3;

template<typename T>
class Normal3;

using Vector2f = Vector2<Float>;
using Vector3f = Vector3<Float>;
using Vector2i = Vector2<Integer>;
using Point2f = Point2<Float>;
using Point2i = Point2<Integer>;
using Point3f = Point3<Float>;
using Point3i = Point3<Integer>;
using Normal3f = Normal3<Float>;

template<template<typename> class Child, typename T>
class Tuple2 {
public:
    T x{}, y{};

    Tuple2() = default;

    Tuple2(T v) : x{v}, y{v} {}

    Tuple2(T x, T y) : x{x}, y{y} {}

    template<typename U>
    auto operator+(Child<U> t) const -> Child<decltype(T{} + U{})> {
        return {x + t.x, y + t.y};
    }

    template<typename U>
    Child<T> &operator+=(Child<U> t) {
        x += t.x;
        y += t.y;
        return static_cast<Child<T> &>(*this);
    }

    auto operator-() const -> Child<decltype(-T{})> {
        return {-x, -y};
    }

    template<typename U>
    auto operator-(Child<U> t) const -> Child<decltype(T{} - U{})> {
        return {x - t.x, y - t.y};
    }

    template<typename U>
    Child<T> &operator-=(Child<U> t) {
        x -= t.x;
        y -= t.y;
        return static_cast<Child<T> &>(*this);
    }

    template<typename U>
    auto operator*(U s) const -> Child<decltype(T{} * U{})> {
        return {s * x, s * y};
    }

    template<typename U>
    Child<T> &operator*=(U s) {
        x *= s;
        y *= s;
        return static_cast<Child<T> &>(*this);
    }

    template<typename U>
    auto operator*(Child<U> t) const -> Child<decltype(T{} * U{})> {
        return {x * t.x, y * t.y};
    }

    template<typename U>
    Child<T> &operator*=(Child<U> t) {
        x *= t.x;
        y *= t.y;
        return static_cast<Child<T> &>(*this);
    }

    template<typename U>
    auto operator/(U s) const -> Child<decltype(T{} / U{})> {
        assert(s != 0);
        return {x / s, y / s};
    }

    template<typename U>
    Child<T> &operator/=(U s) {
        assert(s != 0);
        x /= s;
        y /= s;
        return static_cast<Child<T> &>(*this);
    }

    template<typename U>
    auto operator/(Child<U> t) const -> Child<decltype(T{} / U{})> {
        assert(t.x != 0 && t.y != 0);
        return {x / t.x, y / t.y};
    }

    template<typename U>
    Child<T> &operator/=(Child<U> t) {
        assert(t.x != 0 && t.y != 0);
        x /= t.x;
        y /= t.y;
        return static_cast<Child<T> &>(*this);
    }

    T operator[](Integer i) const {
        assert(i >= 0 && i < 2);
        if (i == 0) {
            return x;
        }
        return y;
    }

    T &operator[](Integer i) {
        assert(i >= 0 && i < 2);
        if (i == 0) {
            return x;
        }
        return y;
    }

    Boolean operator==(Child<T> t) const {
        return x == t.x && y == t.y;
    }

    Boolean operator!=(Child<T> t) const {
        return x != t.x || y != t.y;
    }
};

template<template<class> class C, typename T, typename U>
inline auto operator*(U s, Tuple2<C, T> t) -> C<decltype(T{} * U{})> {
    return t * s;
}

template<template<class> class C, typename T>
inline C<T> Min(Tuple2<C, T> t0, Tuple2<C, T> t1) {
    return {std::min(t0.x, t1.x), std::min(t0.y, t1.y)};
}

template<template<class> class C, typename T>
inline C<T> Max(Tuple2<C, T> t0, Tuple2<C, T> t1) {
    return {std::max(t0.x, t1.x), std::max(t0.y, t1.y)};
}

template<template<class> class C, typename T>
inline C<T> Abs(Tuple2<C, T> t) {
    return {std::abs(t.x), std::abs(t.y)};
}

template<template<typename> class Child, typename T>
class Tuple3 {
public:
    T x{}, y{}, z{};

    Tuple3() = default;

    Tuple3(T v) : x{v}, y{v}, z{v} {}

    Tuple3(T x, T y, T z) : x{x}, y{y}, z{z} {}

    template<typename U>
    auto operator+(Child<U> t) const -> Child<decltype(T{} + U{})> {
        return {x + t.x, y + t.y, z + t.z};
    }

    template<typename U>
    Child<T> &operator+=(Child<U> t) {
        x += t.x;
        y += t.y;
        z += t.z;
        return static_cast<Child<T> &>(*this);
    }

    auto operator-() const -> Child<decltype(-T{})> {
        return {-x, -y, -z};
    }

    template<typename U>
    auto operator-(Child<U> t) const -> Child<decltype(T{} - U{})> {
        return {x - t.x, y - t.y, z - t.z};
    }

    template<typename U>
    Child<T> &operator-=(Child<U> t) {
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return static_cast<Child<T> &>(*this);
    }

    template<typename U>
    auto operator*(U s) const -> Child<decltype(T{} * U{})> {
        return {s * x, s * y, s * z};
    }

    template<typename U>
    Child<T> &operator*=(U s) {
        x *= s;
        y *= s;
        z *= s;
        return static_cast<Child<T> &>(*this);
    }

    template<typename U>
    auto operator*(Child<U> t) const -> Child<decltype(T{} * U{})> {
        return {x * t.x, y * t.y, z * t.z};
    }

    template<typename U>
    Child<T> &operator*=(Child<U> t) {
        x *= t.x;
        y *= t.y;
        z *= t.z;
        return static_cast<Child<T> &>(*this);
    }

    template<typename U>
    auto operator/(U s) const -> Child<decltype(T{} / U{})> {
//        std::cout << std::stacktrace().current();
//        assert(s != 0);
        return {x / s, y / s, z / s};
    }

    template<typename U>
    Child<T> &operator/=(U s) {
        assert(s != 0);
        x /= s;
        y /= s;
        z /= s;
        return static_cast<Child<T> &>(*this);
    }

    template<typename U>
    auto operator/(Child<U> t) const -> Child<decltype(T{} / U{})> {
        assert(t.x != 0 && t.y != 0 && t.z != 0);
        return {x / t.x, y / t.y, z / t.z};
    }

    template<typename U>
    Child<T> &operator/=(Child<U> t) {
        assert(t.x != 0 && t.y != 0 && t.z != 0);
        x /= t.x;
        y /= t.y;
        z /= t.z;
        return static_cast<Child<T> &>(*this);
    }

    T operator[](Integer i) const {
        assert(i >= 0 && i < 3);
        if (i == 0) {
            return x;
        }
        if (i == 1) {
            return y;
        }
        return z;
    }

    T &operator[](Integer i) {
        assert(i >= 0 && i < 3);
        if (i == 0) {
            return x;
        }
        if (i == 1) {
            return y;
        }
        return z;
    }

    Boolean operator==(Child<T> t) const {
        return x == t.x && y == t.y && z == t.z;
    }

    Boolean operator!=(Child<T> t) const {
        return x != t.x || y != t.y || z != t.z;
    }
};

template<template<class> class C, typename T, typename U>
inline auto operator*(U s, Tuple3<C, T> t) -> C<decltype(T{} * U{})> {
    return t * s;
}

template<template<class> class C, typename T>
inline C<T> Min(Tuple3<C, T> t0, Tuple3<C, T> t1) {
    return {std::min(t0.x, t1.x), std::min(t0.y, t1.y), std::min(t0.z, t1.z)};
}

template<template<class> class C, typename T>
inline C<T> Max(Tuple3<C, T> t0, Tuple3<C, T> t1) {
    return {std::max(t0.x, t1.x), std::max(t0.y, t1.y), std::max(t0.z, t1.z)};
}

template<template<class> class C, typename T>
inline C<T> Abs(Tuple3<C, T> t) {
    return {std::abs(t.x), std::abs(t.y), std::abs(t.z)};
}

template<typename T>
class Vector2 : public Tuple2<Vector2, T> {
public:
    using Tuple2<Vector2, T>::x;
    using Tuple2<Vector2, T>::y;

    Vector2() = default;

    Vector2(T v) : Tuple2<Vector2, T>(v) {}

    Vector2(T x, T y) : Tuple2<Vector2, T>(x, y) {}
};

template<typename T>
inline auto Dot(Vector2<T> v1, Vector2<T> v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

template<typename T>
inline auto LengthSquared(Vector2<T> v) {
    return Dot(v, v);
}

template<typename T>
inline auto Length(Vector2<T> v) {
    return std::sqrt(LengthSquared(v));
}

template<typename T>
inline auto Normalize(Vector2<T> v) {
    return v / Length(v);
}

template<typename T>
class Vector3 : public Tuple3<Vector3, T> {
public:
    using Tuple3<Vector3, T>::x;
    using Tuple3<Vector3, T>::y;
    using Tuple3<Vector3, T>::z;

    Vector3() = default;

    Vector3(T v) : Tuple3<Vector3, T>{v, v, v} {}

    Vector3(T x, T y, T z) : Tuple3<Vector3, T>{x, y, z} {}

    template<typename U>
    explicit Vector3(Vector3<U> v) : Tuple3<Vector3, T>{T(v.x), T(v.y), T(v.z)} {}

    template<typename U>
    explicit Vector3(Point3<U> p) : Tuple3<Vector3, T>{T(p.x), T(p.y), T(p.z)} {}

    template<typename U>
    explicit Vector3(Normal3<U> n) : Tuple3<Vector3, T>{T(n.x), T(n.y), T(n.z)} {}
};

template<typename T>
inline auto Dot(Vector3<T> v1, Vector3<T> v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<typename T>
inline Vector3<T> Cross(Vector3<T> v1, Vector3<T> v2) {
    return Vector3<T>{v1.y * v2.z - v1.z * v2.y,
                      v1.z * v2.x - v1.x * v2.z,
                      v1.x * v2.y - v1.y * v2.x};
}

template<typename T>
inline auto LengthSquared(Vector3<T> v) {
    return Dot(v, v);
}

template<typename T>
inline auto Length(Vector3<T> v) {
    return std::sqrt(LengthSquared(v));
}

template<typename T>
inline auto Normalize(Vector3<T> v) {
    return v / Length(v);
}

template<typename T>
class Point2 : public Tuple2<Point2, T> {
public:
    using Tuple2<Point2, T>::x;
    using Tuple2<Point2, T>::y;
    using Tuple2<Point2, T>::operator+;
    using Tuple2<Point2, T>::operator+=;
    using Tuple2<Point2, T>::operator*;
    using Tuple2<Point2, T>::operator*=;

    Point2() = default;

    Point2(T v) : Tuple2<Point2, T>{v} {}

    Point2(T x, T y) : Tuple2<Point2, T>{x, y} {}

    template<typename U>
    explicit Point2(Point2<U> v) : Tuple2<Point2, T>{T(v.x), T(v.y)} {}

    template<typename U>
    explicit Point2(Vector2<U> v) : Tuple2<Point2, T>{T(v.x), T(v.y)} {}

    template<typename U>
    auto operator+(Vector2<U> v) const -> Point2<decltype(T{} + U{})> {
        return {x + v.x, y + v.y};
    }

    template<typename U>
    Point2<T> &operator+=(Vector2<U> v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    template<typename U>
    auto operator-(Point2<U> p) const -> Vector2<decltype(T{} - U{})> {
        return {x - p.x, y - p.y};
    }

    template<typename U>
    auto operator-(Vector2<U> v) const -> Point2<decltype(T{} - U{})> {
        return {x - v.x, y - v.y};
    }

    template<typename U>
    Point2<T> &operator-=(Vector2<U> v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
};

template<typename T>
class Point3 : public Tuple3<Point3, T> {
public:
    using Tuple3<Point3, T>::x;
    using Tuple3<Point3, T>::y;
    using Tuple3<Point3, T>::z;
    using Tuple3<Point3, T>::operator+;
    using Tuple3<Point3, T>::operator+=;
    using Tuple3<Point3, T>::operator*;
    using Tuple3<Point3, T>::operator*=;

    Point3() = default;

    Point3(T v) : Tuple3<Point3, T>{v} {}

    Point3(T x, T y, T z) : Tuple3<Point3, T>{x, y, z} {}

    template<typename U>
    explicit Point3(Point3<U> p) : Tuple3<Point3, T>{T(p.x), T(p.y), T(p.z)} {}

    template<typename U>
    explicit Point3(Vector3<U> v) : Tuple3<Point3, T>{T(v.x), T(v.y), T(v.z)} {}

    template<typename U>
    auto operator+(Vector3<U> v) const -> Point3<decltype(T{} + U{})> {
        return {x + v.x, y + v.y, z + v.z};
    }

    template<typename U>
    Point3<T> &operator+=(Vector3<U> v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template<typename U>
    auto operator-(Point3<U> p) const -> Vector3<decltype(T{} - U{})> {
        return {x - p.x, y - p.y, z - p.z};
    }

    template<typename U>
    auto operator-(Vector3<U> v) const -> Point3<decltype(T{} - U{})> {
        return {x - v.x, y - v.y, z - v.z};
    }

    template<typename U>
    Point3<T> &operator-=(Vector3<U> v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
};

template<typename T>
class Normal3 : public Tuple3<Normal3, T> {
public:
    using Tuple3<Normal3, T>::x;
    using Tuple3<Normal3, T>::y;
    using Tuple3<Normal3, T>::z;
    using Tuple3<Normal3, T>::operator+;
    using Tuple3<Normal3, T>::operator*;
    using Tuple3<Normal3, T>::operator*=;

    Normal3() = default;

    Normal3(T x, T y, T z) : Tuple3<Normal3, T>{x, y, z} {}

    template<typename U>
    explicit Normal3<T>(Normal3<U> n) : Tuple3<Normal3, T>{T(n.x), T(n.y), T(n.z)} {}

    template<typename U>
    explicit Normal3(Point3<U> v) : Tuple3<Normal3, T>{T(v.x), T(v.y), T(v.z)} {}

    template<typename U>
    explicit Normal3(Vector3<U> v) : Tuple3<Normal3, T>{T(v.x), T(v.y), T(v.z)} {}
};

template<typename T>
inline auto LengthSquared(Normal3<T> n) {
    return n.x * n.x + n.y * n.y + n.z * n.z;
}

template<typename T>
inline auto Length(Normal3<T> n) {
    return std::sqrt(LengthSquared(n));
}

template<typename T>
inline Normal3<T> Normalize(Normal3<T> n) {
    return n / Length(n);
}


NAMESPACE_END
