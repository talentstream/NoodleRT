//
// Created by talentstream on 2024/2/28.
//

#include "base/light.h"
#include "base/shape.h"
#include "core/record.h"
#include <print>

NAMESPACE_BEGIN
class AreaLight : public Emitter {
public:
    explicit AreaLight(const PropertyList &propList) {
        mIntensity = propList.GetColor("intensity", {1});
        mTwoSided = propList.GetBoolean("twosided", false);
    }

    Color3f L(const IntersectionRecord &si, const Vector3f &w) const override {
        return (mTwoSided || Dot(si.n, w) > 0) ? mIntensity : Color3f{};
    }

    Color3f SampleLi(const IntersectionRecord &si, Vector3f &wi, Point2f &sample) const override {
        return {1.f};
    }

    Color3f Sample_Li(EmitterRecord &lRec, const Point2f &u) const override {
        ShapeRecord sRec{lRec.ref};
        pShape->Sample(sRec, u);

        lRec.p = sRec.p;
        lRec.n = sRec.n;
        lRec.wi = Normalize(lRec.p - lRec.ref);// light -> point
        lRec.pdf = sRec.pdf;

        return mIntensity / lRec.pdf;
    }

    LightFlag
    Flag() const override {
        return LightFlag::EArea;
    }

    void
    SetParent(Object *parent) override {
        switch (parent->GetClassType()) {
            case EClassType::EShape:
                pShape = dynamic_cast<Shape *>(parent);
                break;
            default:
                /* throw exception*/
                break;
        }
    }

    void Initialize() {
        if (pShape == nullptr) {
            /* throw exception */
        }
    }

private:
    Shape *pShape{nullptr};
    Color3f mIntensity;
    Boolean mTwoSided;
};

REGISTER_CLASS(AreaLight, "area")

NAMESPACE_END