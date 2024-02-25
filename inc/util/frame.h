//
// Created by talentstream on 2024/2/25.
//

#pragma once

#include "core/math.h"

NAMESPACE_BEGIN

static auto CoordinateSystem(const Vector3f &n) -> std::pair<Vector3f, Vector3f> {
    Vector3f s, t;
    if (Abs(n.x) > Abs(n.y)) {
        Float invLen = 1 / Sqrt(n.x * n.x + n.z * n.z);
        t = {n.z * invLen, 0, -n.x * invLen};
    } else {
        Float invLen = 1 / Sqrt(n.y * n.y + n.z * n.z);
        t = {0, n.z * invLen, -n.y * invLen};
    }
    s = Cross(t, n);
    s = Normalize(s);
    t = Normalize(t);
    return {s, t};
}

struct Frame {
    Vector3f s, t;
    Normal3f n;

    Frame() = default;

    explicit Frame(Vector3f v)
            : n(Normalize(v)) {
        std::tie(s, t) = CoordinateSystem(v);
    }

    explicit Frame(Normal3f v)
            : n(Normalize(v)) {
        std::tie(s, t) = CoordinateSystem(Vector3f{v});
    }

    explicit Frame(Vector3f s, Vector3f t, Vector3f n)
            : s(s), t(t), n(n) {}

    [[nodiscard]] Vector3f ToLocal(Vector3f v) const {
        return {Dot(v, s), Dot(v, t), Dot(v, n)};
    }

    [[nodiscard]] Vector3f ToWorld(Vector3f v) const {
        return s * v.x + t * v.y + Vector3f{n} * v.z;
    }

    static Float CosTheta(Vector3f w) {
        return w.z;
    }

    static Float SinTheta(Vector3f w) {
        return Sqrt(Max(0, SinTheta2(w)));
    }

    static Float TanTheta(Vector3f w) {
        return Sqrt(Max(0, 1 - Sqr(w.z))) / w.z;
    }

    static Float SinTheta2(Vector3f w) {
        return 1 - Sqr(w.z);
    }

    static Float SinPhi(Vector3f w) {
        Float sinTheta = SinTheta(w);
        if (sinTheta == 0) {
            return 1;
        }
        return Clamp(w.y / sinTheta, -1.0f, 1.0f);
    }

    static Float CosPhi(Vector3f w) {
        Float sinTheta = SinTheta(w);
        if (sinTheta == 0) {
            return 1;
        }
        return Clamp(w.x / sinTheta, -1.0f, 1.0f);
    }

    static Float SinPhi2(Vector3f w) {
        return Clamp(Sqr(w.y) / SinTheta2(w), 0.0f, 1.0f);
    }

    static Float CosPhi2(Vector3f w) {
        return Clamp(Sqr(w.x) / SinTheta2(w), 0.0f, 1.0f);
    }

    Boolean operator==(const Frame &rhs) const {
        return s == rhs.s &&
               t == rhs.t &&
               n == rhs.n;
    }

    Boolean operator!=(const Frame &rhs) const {
        return !(rhs == *this);
    }
};

NAMESPACE_END