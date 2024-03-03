//
// Created by talentstream on 2024/2/28.
//

#include "base/light.h"
#include "base/shape.h"
#include "core/record.h"
#include <print>

NAMESPACE_BEGIN
class AreaLight : public Light {
public:
    explicit AreaLight(const PropertyList &propList) {
        mIntensity = propList.GetColor("intensity", {1});
    }

    Color3f L(const SurfaceInteraction &si, const Vector3f &w) const override {
        return Dot(si.n, w) > 0 ? mIntensity : Color3f{};
    }

    Color3f SampleLi(const SurfaceInteraction &si, Vector3f &wi, Point2f &sample) const override {
        return {0.f};
    }

    Color3f Sample_Li(LightSampleRecord &lRec) const override {
        ShapeSampleRecord sRec{lRec.si.p};
        pShape->Sample(sRec, lRec.sampler->Next2D());
        lRec.wi = Normalize(sRec.p - lRec.si.p);
        lRec.pdf = sRec.pdf;
        return Dot(sRec.n, -lRec.wi) > 0 ? mIntensity : Color3f{};
    }

    LightFlag
    Flag() const override {
        return LightFlag::EArea;
    }

    void AddChild(Object *child) override {
        switch (child->GetClassType()) {
            case EClassType::EShape:
                pShape = dynamic_cast<Shape *>(child);
                break;
            default:
                break;
        }
    }

private:
    Shape *pShape{nullptr};
    Color3f mIntensity;
};

REGISTER_CLASS(AreaLight, "area")

NAMESPACE_END