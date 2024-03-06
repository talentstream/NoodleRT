//
// Created by 44772 on 2024/2/13.
//

#include "base/light.h"

NAMESPACE_BEGIN

class PointLight : public Emitter {
public:
    explicit PointLight(const PropertyList &propList) {
        mPosition = propList.GetPoint("position", {});
        mIntensity = propList.GetColor("intensity", {1});
    }

    Color3f
    SampleLi(EmitterRecord &lRec, const Point2f &sample) const override {

        lRec.p = mPosition;
        lRec.n = Normal3f{lRec.wi};
        lRec.wi = Normalize(lRec.p - lRec.ref);

        lRec.pdf = 1.f;
        return mIntensity / LengthSquared(mPosition - lRec.ref);
    }

    LightFlag Flag() const override {
        return LightFlag::EDeltaPosition;
    }

private:
    Point3f mPosition;
    Color3f mIntensity;
};

REGISTER_CLASS(PointLight, "point")

NAMESPACE_END