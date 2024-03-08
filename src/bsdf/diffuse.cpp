//
// Created by 44772 on 2024/2/2.
//

#include "base/bsdf.h"
#include "base/texture.h"
#include "util/sampling.h"
#include <print>

NAMESPACE_BEGIN

// Ideal Diffuse, Lambertian
// Represents an ideally diffuse material with a user-specified amount of reflectance.
class Diffuse : public BSDF {
public:
    explicit Diffuse(const PropertyList &propertyList) {
        mReflectance = propertyList.GetColor("reflectance", {1.f});
        PRINT_DEBUG_INFO("BxDF", "diffuse")
    }

    Color3f
    Eval(const BxDFRecord &bRec) const override {
        if (Frame::CosTheta(bRec.wo) <= 0 ||
            Frame::CosTheta(bRec.wi) <= 0) {
            return {0.f};
        }

        return pAlbedo->Evaluate(bRec.uv) * Frame::CosTheta(bRec.wo) * InvPi;

    }

    Float Pdf(const BxDFRecord &bRec) const override {
        if (Frame::CosTheta(bRec.wo) * Frame::CosTheta(bRec.wi) < 0) {
            return 0.f;
        }
        return Warp::SquareToCosineHemispherePdf(bRec.wo);
    }

    Color3f
    Sample(BxDFRecord &bRec, Float &pdf, const Point2f &sample) const override {
        if (Frame::CosTheta(bRec.wi) <= 0) return {0.f};

        bRec.wo = Warp::SquareToCosineHemisphere(sample);

        pdf = Warp::SquareToCosineHemispherePdf(bRec.wo);

        return pAlbedo->Evaluate(bRec.uv);
    }

    void AddChild(Object *object) override {
        switch (object->GetClassType()) {
            case EClassType::ETexture:
                pAlbedo = dynamic_cast<Texture *>(object);
                break;
        }
    }

    BxDFFlag Flag() const override {
        return EDiffuse;
    }

    void Initialize() override {
        if (pAlbedo == nullptr) {
            pAlbedo = dynamic_cast<Texture *>(ObjectFactory::CreateInstance("solid", {}, true));
        }
    }

private:
    Color3f mReflectance;
    Texture *pAlbedo{nullptr};
};


REGISTER_CLASS(Diffuse, "diffuse")

NAMESPACE_END