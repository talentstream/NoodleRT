//
// Created by talentstream on 2024/3/2.
//

#include "base/bxdf.h"
#include "base/texture.h"
#include "util/sampling.h"
#include <print>

NAMESPACE_BEGIN

// Rough Diffuse, Oren-Nayar
//
class RoughDiffuse : public BxDF {
public:
    explicit RoughDiffuse(const PropertyList &propertyList) {
        mAlbedo = propertyList.GetColor("albedo", {0.5f});
        mRoughness = propertyList.GetFloat("roughness", 0.5f);
        Float r2 = mRoughness * mRoughness;
        A = 1 - (r2 / (2 * (r2 + 0.33f)));
        B = 0.45f * r2 / (r2 + 0.09f);
        PRINT_DEBUG_INFO("BxDF", "roughdiffuse")
    }

    Color3f
    F(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const override {
        if (Frame::CosTheta(wo) <= 0 ||
            Frame::CosTheta(wi) <= 0) {
            return {0.f};
        }

        Float sinThetaI = Frame::SinTheta(wi);
        Float sinThetaO = Frame::SinTheta(wo);

        Float maxCos{0};
        if (sinThetaI > Epsilon && sinThetaO > Epsilon) {
            Float sinPhiI = Frame::SinPhi(wi),
                    cosPhiI = Frame::CosPhi(wi),
                    sinPhiO = Frame::SinPhi(wo),
                    cosPhiO = Frame::CosPhi(wo);
            Float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
            maxCos = Max(0, dCos);
        }
        Float sinAlpha, tanBeta;
        if (Frame::AbsCosTheta(wi) > Frame::AbsCosTheta(wo)) {
            sinAlpha = sinThetaO;
            tanBeta = sinThetaI / Frame::AbsCosTheta(wi);
        } else {
            sinAlpha = sinThetaI;
            tanBeta = sinThetaO / Frame::AbsCosTheta(wo);
        }

        return mAlbedo * InvPi * Frame::CosTheta(wo) * (A + B * maxCos * sinAlpha * tanBeta);
    }

    Float
    Pdf(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const override {
        if (Frame::CosTheta(wo) <= 0 ||
            Frame::CosTheta(wi) <= 0) {
            return 0.f;
        }

        return SquareToCosineHemispherePdf(wo);
    }

    std::optional<Color3f>
    SampleF(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        if (Frame::CosTheta(wo) < 0) return std::nullopt;

        wi = SquareToCosineHemisphere(sample);
        // (brdf / pdf) * cos = [(albedo / pi) / (cos / pi)] * cos
        return mAlbedo;
    }

    BxDFFlag Flag() const override {
        return EDiffuseReflection;
    }

private:
    Color3f mAlbedo;
    Float mRoughness;
    Float A, B;

};

REGISTER_CLASS(RoughDiffuse, "roughdiffuse")

NAMESPACE_END