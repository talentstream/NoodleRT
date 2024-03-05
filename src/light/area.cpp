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
    }

    Color3f L(const IntersectionRecord &si, const Vector3f &w) const override {
        return Dot(si.n, w) > 0 ? mIntensity : Color3f{};
    }

    Color3f SampleLi(const IntersectionRecord &si, Vector3f &wi, Point2f &sample) const override {
        return {1.f};
    }

    Color3f Sample_Li(LightSampleRecord &lRec) const override {
        ShapeSampleRecord sRec{lRec.si.p};
        pShape->Sample(sRec, lRec.sampler->Next2D());
        lRec.wi = Normalize(sRec.p - lRec.si.p);// light -> point
        lRec.pdf = sRec.pdf;
        return Dot(sRec.n, -lRec.wi) > 0 ? mIntensity : Color3f{};
    }

    LightFlag
    Flag() const override {
        return LightFlag::EArea;
    }

    void
    SetParent(Object *parent) override {
        switch (parent->GetClassType()) {
            case EClassType::EShape:
                pShape = dynamic_cast<shape *>(parent);
                break;
            default:
                /* throw exception*/
                break;
        }
    }

    void Initialize() {
        if(pShape == nullptr){
            /* throw exception */
        }
    }

private:
    shape *pShape{nullptr};
    Color3f mIntensity;
};

REGISTER_CLASS(AreaLight, "area")

NAMESPACE_END