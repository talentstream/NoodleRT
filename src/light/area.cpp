//
// Created by talentstream on 2024/2/28.
//

#include "base/light.h"
#include "base/shape.h"
#include <print>

NAMESPACE_BEGIN
class AreaLight : public Light {
public:
    explicit AreaLight(const PropertyList &propList) {
        mIntensity = propList.GetColor("intensity", {1});
    }

    Color3f L(const SurfaceInteraction &si, const Vector3f &w) const override {
        return mIntensity;
    }

    Color3f SampleLi(const SurfaceInteraction &si, Vector3f &wi, Point2f &sample) const override {
        return {0.f};
    }

private:
    Color3f mIntensity;
};

REGISTER_CLASS(AreaLight, "area")

NAMESPACE_END