//
// Created by talentstream on 2024/3/2.
//

#include "base/bsdf.h"
#include "base/texture.h"
#include "util/sampling.h"
#include <print>

NAMESPACE_BEGIN

// Rough Diffuse, Oren-Nayar
//
class RoughDiffuse : public BSDF {
public:
    explicit RoughDiffuse(const PropertyList &propertyList) {
        mReflectance = propertyList.GetColor("albedo", {1.f});
        mRoughness = propertyList.GetFloat("roughness", 0.5f);
        Float r2 = mRoughness * mRoughness;
        A = 1 - (r2 / (2 * (r2 + 0.33f)));
        B = 0.45f * r2 / (r2 + 0.09f);
        PRINT_DEBUG_INFO("BxDF", "roughdiffuse")
    }

    Color3f
    Eval(const BxDFRecord &bRec) const override {
        if (Frame::CosTheta(bRec.wo) <= 0 ||
            Frame::CosTheta(bRec.wi) <= 0) {
            return {0.f};
        }

        Float sinThetaI = Frame::SinTheta(bRec.wi);
        Float sinThetaO = Frame::SinTheta(bRec.wo);

        Float maxCos{0};
        if (sinThetaI > Epsilon && sinThetaO > Epsilon) {
            Float sinPhiI = Frame::SinPhi(bRec.wi),
                    cosPhiI = Frame::CosPhi(bRec.wi),
                    sinPhiO = Frame::SinPhi(bRec.wo),
                    cosPhiO = Frame::CosPhi(bRec.wo);
            Float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
            maxCos = Max(0, dCos);
        }
        Float sinAlpha, tanBeta;
        if (Frame::AbsCosTheta(bRec.wi) > Frame::AbsCosTheta(bRec.wo)) {
            sinAlpha = sinThetaO;
            tanBeta = sinThetaI / Frame::AbsCosTheta(bRec.wi);
        } else {
            sinAlpha = sinThetaI;
            tanBeta = sinThetaO / Frame::AbsCosTheta(bRec.wo);
        }

        return mReflectance * InvPi * (A + B * maxCos * sinAlpha * tanBeta);

    }

    Float
    Pdf(const BxDFRecord &bRec) const override {
        if (Frame::CosTheta(bRec.wo) <= 0 ||
            Frame::CosTheta(bRec.wi) <= 0) {
            return 0.f;
        }
        return Warp::SquareToCosineHemispherePdf(bRec.wo);
    }


    Color3f
    Sample(BxDFRecord &bRec, Float &pdf, const Point2f &sample) const override {
        if (Frame::CosTheta(bRec.wi) <= 0) return {0.f};

        bRec.wo = Warp::SquareToCosineHemisphere(sample);

        pdf = Warp::SquareToCosineHemispherePdf(bRec.wo);

        return Eval(bRec) / pdf;
    }

    BxDFFlag
    Flag() const override {
        return EDiffuseReflection;
    }

private:
    Color3f mReflectance;
    Float mRoughness;
    Float A, B;

};

REGISTER_CLASS(RoughDiffuse, "roughdiffuse")

NAMESPACE_END