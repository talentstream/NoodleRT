//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include "math/vec.h"
#include "math/ray.h"
#include "math/interval.h"

NAMESPACE_BEGIN

template<typename T>
class Bound2;

template<typename T>
class Bound3;

using Bound2i = Bound2<Integer>;
using Bound3f = Bound3<Float>;

template<typename T>
class Bound2 {
public:
    Point2<T> pMin, pMax;

    Bound2() {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();
        pMin = Point2<T>{maxNum, maxNum};
        pMax = Point2<T>{minNum, minNum};
    }

    explicit Bound2(Point2<T> p) : pMin{p}, pMax{p} {}

    explicit Bound2(Point2<T> p1, Point2<T> p2) : pMin{Min(p1, p2)}, pMax{Max(p1, p2)} {}

    T Area() const {
        auto d = pMax - pMin;
        return d.x * d.y;
    }

};

template<typename T>
class Bound3 {
public:
    Point3<T> pMin, pMax;

    Bound3() {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();
        pMin = Point3<T>{maxNum, maxNum, maxNum};
        pMax = Point3<T>{minNum, minNum, minNum};
    }

    explicit Bound3(Point3<T> p) : pMin{p}, pMax{p} {}

    explicit Bound3(Point3<T> p1, Point3<T> p2) : pMin{Min(p1, p2)}, pMax{Max(p1, p2)} {}

    Bound3<T> operator+(Point3<T> p) const {
        return Bound3<T>{pMin + p, pMax + p};
    }

    [[nodiscard]] Boolean IntersectP(const Ray &ray, Float tMax) const {
        Float t0{0.001}, t1{tMax};
        for (const Integer axis: {0, 1, 2}) {
            auto invD = 1 / ray.d[axis];
            auto o = ray.o[axis];
            auto tNear = (pMin[axis] - o) * invD;
            auto tFar = (pMax[axis] - o) * invD;
            if (invD < 0) {
                std::swap(tNear, tFar);
            }
            t0 = tNear > t0 ? tNear : t0;
            t1 = tFar < t1 ? tFar : t1;
            if (t0 > t1) return false;
        }
        return true;
    }

    Bound3<T>
    Expand(Point3<T> p) {
        return Bound3<T>{Min(pMin, p), Max(pMax, p)};
    }

    Bound3<T>
    Expand(Bound3<T> b) {
        return Bound3<T>{Min(pMin, b.pMin), Max(pMax, b.pMax)};
    }
};

template<typename T>
inline Bound3<T> operator+(Point3<T> p, const Bound3<T> &b) {
    return b + p;
}

template<typename T>
inline Bound3<T> Union(const Bound3<T> &b1, const Bound3<T> &b2) {
    return Bound3<T>{Min(b1.pMin, b2.pMin), Max(b1.pMax, b2.pMax)};
}

template<typename T>
inline Bound3<T> Union(const Bound3<T> &b, const Point3<T> &p) {
    return Bound3<T>{Min(b.pMin, p), Max(b.pMax, p)};
}

template<typename T>
inline Point3<T> Centroid(const Bound3<T> &b) {
    return (b.pMin + b.pMax) * 0.5f;
}

template<typename T>
inline Integer MaxDimension(const Bound3<T> &b) {
    auto d = b.pMax - b.pMin;
    if (d.x > d.y && d.x > d.z) return 0;
    if (d.y > d.z) return 1;
    return 2;
}

template<typename T>
inline Bound3<T> Padding(const Bound3<T> &b, T delta) {
    Vector3f v{delta, delta, delta};
    return Bound3<T>{b.pMin - v, b.pMax + v};
}

NAMESPACE_END
