//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include <cmath>

NAMESPACE_BEGIN
class Interval {
public:
    Float l, r;

    Interval() = default;

    Interval(Float v) : l{v}, r{v} {}

    Interval(Float l, Float r) : l{std::min(l, r)}, r{std::max(l, r)} {}

    explicit Interval(Interval a, Interval b) : l{std::min(a.l, b.l)}, r{std::max(a.r, b.r)} {}

    Interval operator+(Float v) const {
        return {l + v, r + v};
    }

    Boolean InRange(Float v) const {
        return v >= l && v <= r;
    }

    Boolean InRange(Interval i) const {
        return l <= i.l && r >= i.r;
    }

    Float Length() const {
        return r - l;
    }

    Interval Expand(Float v) const {
        Float padding = v / 2;
        return {l - padding, r + padding};
    }

};

NAMESPACE_END
