//
// Created by 44772 on 2024/2/13.
//

#pragma once

#include "core/object.h"
#include "core/interaction.h"

NAMESPACE_BEGIN

enum class LightType {
    EDeltaPosition,
    EDeltaDirection,
    EArea,
    EInfinite
};

inline Boolean IsDeltaLight(LightType type) {
    return (type == LightType::EDeltaPosition ||
            type == LightType::EDeltaDirection);
}

inline Boolean IsAreaLight(LightType type) {
    return (type == LightType::EArea);
}

inline Boolean IsInfiniteLight(LightType type) {
    return (type == LightType::EInfinite);
}

class Light : public Object {
public:
    virtual ~Light() = default;

    // only infinite light has Le
    virtual Color3f Le(const Ray &ray) const {
        return {0};
    }

    // only area light has L
    virtual Color3f
    L(const SurfaceInteraction &si, const Vector3f &w) const {
        return {0};
    }

    virtual Color3f
    SampleLi(const SurfaceInteraction &si, Vector3f &wi, Point2f &sample) const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::ELight;
    }
};


NAMESPACE_END