//
// Created by talentstream on 2024/2/24.
//

#include "base/emitter.h"
#include "util/sampling.h"

NAMESPACE_BEGIN

// Environment light
// radiance & image
class Environment : public Emitter {
public:
    explicit Environment(const PropertyList &propList) {
        mRadiance = propList.GetColor("radiance", {.5f, .7f, 1.f});
    }

    Color3f
    Le(const Ray &ray) const override {
        return mRadiance;
    }

    Color3f
    Sample(EmitterRecord &lRec, const Point2f &sample) const override {
        lRec.wi = Warp::UniformSampleSphere(sample);
        lRec.pdf = Warp::UniformSampleSpherePdf();
        return mRadiance;
    }

    LightFlag
    Flag() const override {
        return LightFlag::EInfinite;
    }

private:
    Color3f mRadiance;
};

REGISTER_CLASS(Environment, "infinite")

NAMESPACE_END