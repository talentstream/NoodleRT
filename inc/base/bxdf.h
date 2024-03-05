//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"
#include "core/record.h"
#include "base/sampler.h"

NAMESPACE_BEGIN

enum BxDFFlag {
    EReflection = 1 << 0,
    ETransmission = 1 << 1,
    EDiffuse = 1 << 2,
    EGlossy = 1 << 3,
    ESpecular = 1 << 4,
    EDiffuseReflection = EDiffuse | EReflection,
    EDiffuseTransmission = EDiffuse | ETransmission,
    EReflectionTransmission = EReflection | ETransmission,
    EAll = EReflection | ETransmission | EDiffuse | EGlossy | ESpecular
};

inline Boolean IsDiffuse(BxDFFlag type) {
    return (type & EDiffuse);
}

inline Boolean IsReflection(BxDFFlag type) {
    return (type & EReflection);
}

inline Boolean IsTransmission(BxDFFlag type) {
    return (type & ETransmission);
}

class BxDF : public Object {
public:
    virtual
    ~BxDF() = default;

    // f bxdf model
    virtual Color3f
    F(const IntersectionRecord &si, const Vector3f wo, const Vector3f wi) const { return {0.f}; }

    virtual Color3f
    Eval(const BxDFRecord &bRec) const { return {0.f}; }

    virtual Float
    Pdf(const IntersectionRecord &si, const Vector3f wo, const Vector3f wi) const { return 0.f; };

    virtual Float
    pdf(const BxDFRecord &bRec) const { return 0.f; };

    virtual std::optional<Color3f>
    SampleF(const IntersectionRecord &si, const Vector3f wo, Vector3f &wi, Point2f sample) const = 0;

    // Sample bxdf & return pdf
    virtual Color3f
    Sample(BxDFRecord &bRec, Float &pdf, const Point2f &sample) const { return {0.f}; };

    virtual BxDFFlag
    Flag() const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EBxDF;
    }
};

NAMESPACE_END
