//
// Created by 44772 on 2024/2/13.
//

#include "base/light.h"

NAMESPACE_BEGIN

class PointLight : public Light {
public:
    explicit PointLight(const PropertyList &propList) {
        mPosition = propList.GetPoint("position", {});
        mIntensity = propList.GetColor("intensity", {1});
    }

    Color3f
    SampleLi(const SurfaceInteraction &si, Vector3f &wi, Point2f &sample) const override {
        wi = Normalize(mPosition - si.p);
        auto d = LengthSquared(mPosition - si.p);
        if (d < Epsilon) return mIntensity;
        return mIntensity / d;
    }

private:
    Point3f mPosition;
    Color3f mIntensity;
};

REGISTER_CLASS(PointLight, "point")

NAMESPACE_END