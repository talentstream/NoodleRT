//
// Created by 44772 on 2024/2/12.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

// Perfect Specular reflection
class Reflection : public BxDF {
public:
    explicit Reflection(const PropertyList &propertyList) {
        mAlbedo = propertyList.GetColor("albedo", {0.5});
        PRINT_DEBUG_INFO("BxDF", "specular")
    }

    Color3f F(const IntersectionRecord &si, const Vector3f wo, const Vector3f wi) const override {
        if(Frame::CosTheta(wo) <= 0 ||
           Frame::CosTheta(wi) <= 0 ||
           Abs(Dot(ReflectLocal(wi),wo)) > Epsilon) {
            return {0.f};
        }

        return mAlbedo;
    }

    Color3f
    Eval(const BxDFSampleRecord &bRec) const override {
        if(Frame::CosTheta(bRec.wo) <= 0 ||
           Frame::CosTheta(bRec.wi) <= 0 ||
           Abs(Dot(ReflectLocal(bRec.wi),bRec.wo)) > Epsilon) {
            return {0.f};
        }

        return mAlbedo;
    }

    Float Pdf(const IntersectionRecord &si, const Vector3f wo, const Vector3f wi) const override {
        if(Frame::CosTheta(wo) <= 0 ||
           Frame::CosTheta(wi) <= 0 ||
           Abs(Dot(ReflectLocal(wi),wo)) > Epsilon) {
            return 0.f;
        }

        return 1.f;
    }

    std::optional<Color3f>
    SampleF(const IntersectionRecord &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        wi = ReflectLocal(wo);

        if (Frame::CosTheta(wo) < 0 ||
            Frame::CosTheta(wi) < 0) {
            return std::nullopt;
        }

        return mAlbedo;
    }

    BxDFFlag Flag() const override{
        return EReflection;
    }

private:
    Color3f mAlbedo;
};

REGISTER_CLASS(Reflection, "reflection")
NAMESPACE_END