//
// Created by 44772 on 2024/2/8.
//

#include "base/sampler.h"

NAMESPACE_BEGIN

class IndependentSampler : public Sampler {
public:
    explicit IndependentSampler(const PropertyList &propertyList) {
        mSampleCount = propertyList.GetInteger("sampleCount", 1);
    }

    virtual ~IndependentSampler() = default;

    [[nodiscard]] Float Next1D() override {
        return rng.UniformFloat();
    }

    [[nodiscard]] Point2f Next2D() override {
        return {rng.UniformFloat(), rng.UniformFloat()};
    }

private:
    Integer mSampleCount;
    RNG rng;
};

NAMESPACE_END