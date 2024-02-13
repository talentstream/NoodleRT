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
       return mIntensity / LengthSquared(mPosition - si.p);
    }

private:
    Point3f mPosition;
    Color3f mIntensity;
};

REGISTER_CLASS(PointLight, "point")

NAMESPACE_END