//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include "math/vec.h"
#include "math/constant.h"
#include <cassert>

NAMESPACE_BEGIN

class Color3f {
public:
    Float r{}, g{}, b{};

    Color3f() = default;

    Color3f(Float v)
            : r{v}, g{v}, b{v} {}

    Color3f(Float r, Float g, Float b)
            : r{r}, g{g}, b{b} {}

    explicit Color3f(Point3f p)
            : r{p.x}, g{p.y}, b{p.z} {}

    explicit Color3f(Vector3f v)
            : r{v.x}, g{v.y}, b{v.z} {}

    explicit Color3f(Normal3f n)
            : r{n.x}, g{n.y}, b{n.z} {}

    Color3f operator+(Color3f c) const {
        return Color3f{r + c.r, g + c.g, b + c.b};
    }

    Color3f &operator+=(Color3f c) {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }

    Color3f operator-(Color3f c) const {
        return Color3f{r - c.r, g - c.g, b - c.b};
    }

    Color3f &operator-=(Color3f c) {
        r -= c.r;
        g -= c.g;
        b -= c.b;
        return *this;
    }

    Color3f operator*(Float s) const {
        return Color3f{r * s, g * s, b * s};
    }

    Color3f &operator*=(Float s) {
        r *= s;
        g *= s;
        b *= s;
        return *this;
    }

    Color3f operator*(Color3f c) const {
        return Color3f{r * c.r, g * c.g, b * c.b};
    }

    Color3f &operator*=(Color3f c) {
        r *= c.r;
        g *= c.g;
        b *= c.b;
        return *this;
    }

    Color3f operator/(Float s) const {
        assert(s != 0);
        return Color3f{r / s, g / s, b / s};
    }

    Color3f &operator/=(Float s) {
        assert(s != 0);
        r /= s;
        g /= s;
        b /= s;
        return *this;
    }

    Color3f operator/(Color3f c) const {
        assert(c.r != 0 && c.g != 0 && c.b != 0);
        return Color3f{r / c.r, g / c.g, b / c.b};
    }

    Color3f &operator/=(Color3f c) {
        assert(c.r != 0 && c.g != 0 && c.b != 0);
        r /= c.r;
        g /= c.g;
        b /= c.b;
        return *this;
    }

    Float &operator[](int i) {
        assert(i >= 0 && i < 3);
        if (i == 0)
            return r;
        if (i == 1)
            return g;
        return b;
    }

    Float operator[](int i) const {
        assert(i >= 0 && i < 3);
        if (i == 0)
            return r;
        if (i == 1)
            return g;
        return b;
    }

    Boolean operator==(Color3f c) const {
        return r == c.r && g == c.g && b == c.b;
    }

    Boolean operator!=(Color3f c) const {
        return r != c.r || g != c.g || b != c.b;
    }

    Color3f LinearToGamma() const {
        return {std::sqrt(r), std::sqrt(g), std::sqrt(b)};
    }

    Boolean IsZero() const {
        return r < Epsilon  && g < Epsilon && b < Epsilon;
    }

};

inline Color3f operator*(Float s, Color3f c) {
    return c * s;
}

inline Float MaxValue(Color3f c){
    return std::max(c.r, std::max(c.g, c.b));
}

NAMESPACE_END
