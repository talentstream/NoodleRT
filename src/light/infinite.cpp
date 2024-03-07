//
// Created by talentstream on 2024/2/24.
//

#include "base/light.h"

NAMESPACE_BEGIN

// Environment light
// this assume to an infinite parallel light
class InfiniteLight : public Emitter {
public:
    explicit InfiniteLight(const PropertyList &propList) {
        mDirection = propList.GetVector("direction", {-1, -1, -1});
        mIntensity = propList.GetColor("intensity", {0.5, 0.7, 1.0});
    }

    Color3f
    Le(const Ray &ray) const override {
        return mIntensity;
    }

    Color3f
    SampleLi(EmitterRecord &lRec, const Point2f &sample) const override {
        lRec.p = Point3f{mDirection} * Infinity;
        lRec.wi = Normalize(mDirection);
        lRec.pdf = 1.f;
        return mIntensity;
    }

    LightFlag
    Flag() const override {
        return LightFlag::EInfinite;
    }

private:
    Vector3f mDirection;
    Color3f mIntensity;
};

REGISTER_CLASS(InfiniteLight, "infinite")

NAMESPACE_END