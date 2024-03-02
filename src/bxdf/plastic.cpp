//
// Created by talentstream on 2024/3/2.
//

#include "base/bxdf.h"
#include "util/sampling.h"
#include <print>

NAMESPACE_BEGIN

// Smooth Plastic, Internal scattering
// Fresnel reflection & transmission coefficients to provide direction-dependent specular and diffuse components.
class Plastic : public BxDF {
public:
    explicit Plastic(const PropertyList &propertyList) {
        mReflectance = propertyList.GetColor("reflectance", {1.f});
        mDiffuse = propertyList.GetColor("diffuse", {.5f});
        Float dSum = mDiffuse[0] + mDiffuse[1] + mDiffuse[2];
        Float sSum = mReflectance[0] + mReflectance[1] + mReflectance[2];
        mSpecularSamplingWeight = sSum / (sSum + dSum);

        Float intIOR = propertyList.GetFloat("intIOR", 1.49f);
        Float extIOR = propertyList.GetFloat("extIOR", 1.00027f);
        mEta = intIOR / extIOR;
        mInvEta2 = 1 / (mEta * mEta);
        mFdrInt = fresnelDiffuse(mEta);
        PRINT_DEBUG_INFO("BxDF", "plastic")
    }

    inline Vector3f reflect(const Vector3f &wi) const {
        return {-wi.x, -wi.y, wi.z};
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

    inline Float fresnelDielectric(Float cosThetaI, Float eta) const {
        Float cosThetaT;
        return fresnelDielectric(cosThetaI, cosThetaT, eta);
    }

    inline Float fresnelDiffuse(Float eta) const {
        if (eta < 1) {
            return -1.4399f * (eta * eta)
                   + 0.7099f * eta
                   + 0.6681f
                   + 0.0636f / eta;
        } else {
            Float invEta = 1 / eta,
                    invEta2 = invEta * invEta,
                    invEta3 = invEta2 * invEta,
                    invEta4 = invEta3 * invEta,
                    invEta5 = invEta4 * invEta;

            return 0.919317f -
                   3.4793f * invEta
                   + 6.75335f * invEta2
                   - 7.80989f * invEta3
                   + 4.98554f * invEta4
                   - 1.36881f * invEta5;
        }
    }

    Color3f
    Sample(BxDFSampleRecord &bRec, Float &pdf, const Point2f &sample) const override {
        if (Frame::CosTheta(bRec.wi) <= 0) {
            return {0.f};
        }

        Float fi = fresnelDielectric(Frame::CosTheta(bRec.wi), mEta);

        // diffuse & reflection
        Float probSpecular = (fi * mSpecularSamplingWeight) /
                             (fi * mSpecularSamplingWeight + (1 - fi) * (1 - mSpecularSamplingWeight));
        if (sample.x < probSpecular) {
            bRec.wo = reflect(bRec.wi);
            pdf = probSpecular;
            return mReflectance * fi / probSpecular;
        } else {
            bRec.wo = Warp::SquareToCosineHemisphere(
                    {(sample.x - probSpecular) / (1 - probSpecular), sample.y});
            pdf = (1 - probSpecular) * Warp::SquareToCosineHemispherePdf(bRec.wo);

            Float fo = fresnelDielectric(Frame::CosTheta(bRec.wo), mEta);
            Color3f diff = mDiffuse / (1 - mFdrInt);

            return diff * (mInvEta2 * (1 - fi) * (1 - fo) / (1 - probSpecular));
        }
    }

    BxDFFlag
    Flag() const override {
        return EDiffuseReflection;
    }

private:
    Color3f mReflectance, mDiffuse;
    Float mEta, mInvEta2;
    Float mFdrInt;
    Float mSpecularSamplingWeight;
};

//REGISTER_CLASS(Plastic, "plastic")

NAMESPACE_END