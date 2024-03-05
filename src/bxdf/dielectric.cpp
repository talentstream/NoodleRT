//
// Created by 44772 on 2024/2/12.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

class Dielectric : public BxDF {
public:
    explicit Dielectric(const PropertyList &propertyList) {
        Float intIOR = propertyList.GetFloat("intIOR", 1.5f);
        Float extIOR = propertyList.GetFloat("extIOR", 1.0f);
        mEta = intIOR / extIOR;
        mInvEta = 1 / mEta;
        mReflectance = propertyList.GetColor("reflectance", {1.0f});
        mTransmittance = propertyList.GetColor("transmittance", {1.0f});
        PRINT_DEBUG_INFO("BxDF", "dielectric")
    }

    inline Vector3f reflect(const Vector3f &wi) const {
        return {-wi.x, -wi.y, wi.z};
    }

    inline Vector3f refract(const Vector3f &wi, Float cosThetaT) const {
        Float scale = -(cosThetaT < 0 ? mInvEta : mEta);
        return {scale * wi.x, scale * wi.y, cosThetaT};
    }

    Color3f F(const IntersectionRecord &si, const Vector3f wo, const Vector3f wi) const override {
        return {.5f};
    }

    inline Float fresnelDielectric(Float cosThetaI_, Float &cosThetaT_, Float eta) const {
        if (eta == 1) {
            cosThetaT_ = -cosThetaI_;
            return 0.f;
        }
        Float scale = (cosThetaI_ > 0) ? 1 / eta : eta;
        Float cosThetaTSqr = 1 - (1 - cosThetaI_ * cosThetaI_) * scale * scale;

        if (cosThetaTSqr <= 0) {
            cosThetaT_ = 0.f;
            return 1.f;
        }

        Float cosThetaI = Abs(cosThetaI_);
        Float cosThetaT = Sqrt(cosThetaTSqr);

        Float Rs = (cosThetaI - eta * cosThetaT) / (cosThetaI + eta * cosThetaT);
        Float Rp = (eta * cosThetaI - cosThetaT) / (eta * cosThetaI + cosThetaT);
        return 0.5f * (Rs * Rs + Rp * Rp);
    }

    Color3f
    Eval(const BxDFSampleRecord &bRec) const override {
        Float cosThetaT;
        Float f = fresnelDielectric(Frame::CosTheta(bRec.wi), cosThetaT, mEta);
        if (Frame::CosTheta(bRec.wi) * Frame::CosTheta(bRec.wo) >= 0) {
            if (Abs(Dot(reflect(bRec.wi), bRec.wo) - 1) > Epsilon) {
                return {0.f};
            }
            return mReflectance * f;
        } else {
            if (Abs(Dot(refract(bRec.wi, cosThetaT), bRec.wo) - 1) > Epsilon) {
                return {0.f};
            }
            Float factor = (cosThetaT < 0) ? mInvEta : mEta;
            return mTransmittance * (1 - f) * Sqr(factor);
        }
    }

    Float Pdf(const IntersectionRecord &si, const Vector3f wo, const Vector3f wi) const override {
        return 1.0f;
    }

    Float pdf(const BxDFSampleRecord &bRec) const override {
        Float cosThetaT;
        Float f = fresnelDielectric(Frame::CosTheta(bRec.wi), cosThetaT, mEta);
        if (Frame::CosTheta(bRec.wi) * Frame::CosTheta(bRec.wo) >= 0) {
            if (Abs(Dot(reflect(bRec.wi), bRec.wo) - 1) > Epsilon) {
                return {0.f};
            }
            return f;
        } else {
            if (Abs(Dot(refract(bRec.wi, cosThetaT), bRec.wo) - 1) > Epsilon) {
                return {0.f};
            }
            return 1 - f;
        }
    }

    std::optional<Color3f>
    SampleF(const IntersectionRecord &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        Float cosThetaI = Min(Frame::CosTheta(wo), 1);
        Float ratio = mEta;
        if (FrDielectric(cosThetaI, ratio) > RandomFloat()) {
            if (RefractLocal(wo, wi, ratio)) {
                return Color3f{.5f};
            }
        }
        wi = ReflectLocal(wo);
        return Color3f{.5f};
    }

    Color3f
    Sample(BxDFSampleRecord &bRec, Float &pdf, const Point2f &sample) const override {
        Float cosThetaT;
        Float f = fresnelDielectric(Frame::CosTheta(bRec.wi), cosThetaT, mEta);
        if(sample.x <= f) {
            bRec.wo = reflect(bRec.wi);
            pdf = f;
            return mReflectance;
        } else {
            bRec.wo = refract(bRec.wi, cosThetaT);
            pdf = 1 - f;
            return mTransmittance;
        }
        return {0.f};
    }

    BxDFFlag Flag() const override {
        return EReflectionTransmission;
    }

private:
    Float mEta, mInvEta;
    Color3f mReflectance, mTransmittance;
};

REGISTER_CLASS(Dielectric, "dielectric")

NAMESPACE_END