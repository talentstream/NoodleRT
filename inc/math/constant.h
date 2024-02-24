//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"

#include <limits>
#include <numbers>

NAMESPACE_BEGIN

constexpr Float Infinity = std::numeric_limits<Float>::infinity();
constexpr Float Epsilon = std::numeric_limits<Float>::epsilon();
constexpr Float Pi = std::numbers::pi_v<Float>;
constexpr Float TwoPi = 2 * Pi;
constexpr Float InvPi = std::numbers::inv_pi_v<Float>;
constexpr Float Sqrt2 = std::numbers::sqrt2_v<Float>;

template<typename T>
inline void Swap(T a, T b){
    std::swap(a,b);
}

inline Float DegreeToRadian(Float degree) {
    return degree * Pi / 180;
}

inline Float RadianToDegree(Float radian) {
    return radian * 180 / Pi;
}

inline Float Lerp(Float t, Float v1, Float v2) {
    return (1 - t) * v1 + t * v2;
}

template<typename T>
inline T Clamp(T v, T low, T high) {
    if (v < low)
        return low;
    if (v > high)
        return high;
    return v;
}

template<Integer N>
inline Float Pow(Float v) {
    if constexpr (N < 0)
        return 1 / Pow<-N>(v);
    Float N2 = Pow<N / 2>(v);
    return N2 * N2 * Pow<N & 1>(v);
}

template<>
inline Float Pow<0>(Float v) {
    return 1;
}

template<>
inline Float Pow<1>(Float v) {
    return v;
}

inline Float Sqr(Float v) {
    return v * v;
}


inline Float Sqrt(Float v) {
    return std::sqrt(v);
}

inline Float Abs(Float v) {
    return std::abs(v);
}

inline Float Dot(Vector3f v, Normal3f n) {
    return Dot(v, Vector3f{n});
}

inline Float Dot(Normal3f n, Vector3f v) {
    return Dot(Vector3f{n}, v);
}

inline Float AbsDot(Vector3f v1, Vector3f v2) {
    return std::abs(Dot(v1, v2));
}

inline Float AbsDot(Vector3f v, Normal3f n) {
    return std::abs(Dot(v, n));
}

inline Float AbsDot(Normal3f n, Vector3f v) {
    return std::abs(Dot(n, v));
}

inline Float Min(Float v1, Float v2) {
    return std::min(v1, v2);
}

inline Float Max(Float v1, Float v2) {
    return std::max(v1, v2);
}

inline Float Tan(Float v) {
    return std::tan(v);
}

inline Integer Floor(Float v) {
    return static_cast<Integer>(std::floor(v));
}

inline Float Cos(Float v) {
    return std::cos(v);
}

inline Float ACos(Float v) {
    return std::acos(v);
}

inline Float Sin(Float v) {
    return std::sin(v);
}

inline Float ASin(Float v) {
    return std::asin(v);
}

inline Float ATan2(Float y, Float x) {
    return std::atan2(y, x);
}

NAMESPACE_END
