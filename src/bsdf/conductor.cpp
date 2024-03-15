//
// Created by talentstream on 2024/3/2.
//

#pragma once

#include "base/bsdf.h"
#include <print>

NAMESPACE_BEGIN

// Smooth Conductor, Perfect Specular reflection
class Conductor : public BSDF {
public:
    explicit Conductor(const PropertyList &propertyList) {
        mReflectance = propertyList.GetColor("reflectance", {1.f});
        mEta = propertyList.GetFloat("eta", 0.f);
        mK = propertyList.GetFloat("k", 1.f);
        PRINT_DEBUG_INFO("BxDF", "conductor")
    }

    inline Vector3f reflect(const Vector3f &wi) const {
        return {-wi.x, -wi.y, wi.z};
    }

    inline Float fresnelConductor(Float cosThetaI, Float eta, Float k) const {
        Float cosThetaI2 = cosThetaI * cosThetaI;
        Float sinThetaI2 = 1 - cosThetaI2;
        Float sinThetaI4 = sinThetaI2 * sinThetaI2;

        Float temp1 = eta * eta - k * k - sinThetaI2;
        Float a2pb2 = SafeSqrt(temp1 * temp1 + 4 * eta * eta * k * k);
        Float a = SafeSqrt((a2pb2 + temp1) * 0.5f);

        Float term1 = a2pb2 + cosThetaI2;
        Float term2 = 2 * a * cosThetaI;

        Float Rs2 = (term1 - term2) / (term1 + term2);
        Float term3 = a2pb2 * cosThetaI2 + sinThetaI4;
        Float term4 = term2 * sinThetaI2;

        Float Rp2 = Rs2 * (term3 - term4) / (term3 + term4);
        return 0.5f * (Rs2 + Rp2);
    }

    Color3f
    Eval(const BxDFRecord &bRec) const override {
        if (Frame::CosTheta(bRec.wo) <= 0 ||
            Frame::CosTheta(bRec.wi) <= 0 ||
            Abs(Dot(reflect(bRec.wi), bRec.wo) - 1) > Epsilon) {
            return {0.f};
        }

        return mReflectance * fresnelConductor(Frame::CosTheta(bRec.wi), mEta, mK);
    }

    Float
    Pdf(const BxDFRecord &bRec) const override {
        if (Frame::CosTheta(bRec.wo) <= 0 ||
            Frame::CosTheta(bRec.wi) <= 0 ||
            Abs(Dot(reflect(bRec.wi), bRec.wo) - 1) > Epsilon) {
            return 0.f;
        }
        return 1.f;
    }

    Color3f
    Sample(BxDFRecord &bRec, Float &pdf, const Point2f &sample) const override {
        if (Frame::CosTheta(bRec.wi) <= 0) return {0.f};

        bRec.wo = reflect(bRec.wi);

        pdf = 1.f;

        return mReflectance * fresnelConductor(Frame::CosTheta(bRec.wi), mEta, mK);
    }

    BxDFFlag Flag() const override {
        return EReflection;
    }

private:
    Float mEta, mK;
    Color3f mReflectance;
};

REGISTER_CLASS(Conductor, "conductor")

NAMESPACE_END