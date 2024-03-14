//
// Created by 44772 on 2024/2/13.
//

#pragma once

#include "core/object.h"
#include "core/record.h"
#include "base/sampler.h"
#include "core/record.h"

NAMESPACE_BEGIN

enum class LightFlag {
    EDeltaPosition,
    EDeltaDirection,
    EArea,
    EInfinite
};

inline Bool IsAreaLight(LightFlag type) {
    return (type == LightFlag::EArea);
}

inline Bool IsInfiniteLight(LightFlag type) {
    return (type == LightFlag::EInfinite);
}


class Emitter : public Object {
public:
    virtual ~Emitter() = default;

    // only infinite light has Le
    virtual Color3f Le(const Ray &ray) const {
        return {0};
    }

    // only area light has L
    virtual Color3f
    L(const IntersectionRecord &si, const Vector3f &w) const {
        return {0.f};
    }

    virtual Color3f
    Sample(EmitterRecord &lRec, const Point2f & sample) const = 0;

    virtual LightFlag
    Flag() const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::ELight;
    }
};


NAMESPACE_END