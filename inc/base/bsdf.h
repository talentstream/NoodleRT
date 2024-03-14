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

inline Bool IsDiffuse(BxDFFlag type) {
    return (type & EDiffuse);
}

inline Bool IsReflection(BxDFFlag type) {
    return (type & EReflection);
}

inline Bool IsTransmission(BxDFFlag type) {
    return (type & ETransmission);
}

class BSDF : public Object {
public:
    virtual
    ~BSDF() = default;

    virtual Color3f
    Eval(const BxDFRecord &bRec) const = 0;

    virtual Float
    Pdf(const BxDFRecord &bRec) const = 0;

    // Sample bxdf & return pdf
    virtual Color3f
    Sample(BxDFRecord &bRec, Float &pdf, const Point2f &sample) const = 0;

    virtual BxDFFlag
    Flag() const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EBxDF;
    }
};

NAMESPACE_END
