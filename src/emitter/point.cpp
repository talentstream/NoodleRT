//
// Created by 44772 on 2024/2/13.
//

#include "base/emitter.h"

NAMESPACE_BEGIN

class Point : public Emitter {
public:
    explicit Point(const PropertyList &propList) {
        mPosition = propList.GetPoint("position", {});
        mRadiance = propList.GetColor("intensity", {1});
    }

    Color3f
    Sample(EmitterRecord &lRec, const Point2f &sample) const override {

        lRec.p = mPosition;
        lRec.n = Normal3f{lRec.wi};
        lRec.wi = Normalize(lRec.p - lRec.ref);

        lRec.pdf = 1.f;
        return mRadiance / LengthSquared(mPosition - lRec.ref);
    }

    LightFlag Flag() const override {
        return LightFlag::EDeltaPosition;
    }

private:
    Point3f mPosition;
    Color3f mRadiance;
};

REGISTER_CLASS(Point, "point")

NAMESPACE_END