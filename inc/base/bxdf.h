//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"
#include "core/interaction.h"
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

struct BxDFSampleRecord {
    const SurfaceInteraction &si;
    Sampler *sampler;

    // shading local coordinate
    Vector3f wi;
    Vector3f wo;

    inline BxDFSampleRecord(const SurfaceInteraction &si, Sampler *sampler, const Vector3f &wi, const Vector3f &wo)
            : si{si}, sampler{sampler}, wi{wi}, wo{wo} {}

    inline BxDFSampleRecord(const SurfaceInteraction &si, const Vector3f &wi, const Vector3f &wo)
            : BxDFSampleRecord(si, nullptr, wi, wo) {}

    inline BxDFSampleRecord(const SurfaceInteraction &si, Sampler *sampler, const Vector3f &wi)
            : BxDFSampleRecord(si, sampler, wi, {}) {}

    inline BxDFSampleRecord(const SurfaceInteraction &si, const Vector3f &wi)
            : BxDFSampleRecord(si, nullptr, wi, {}) {}
};

class BxDF : public Object {
public:
    virtual
    ~BxDF() = default;

    // f bxdf model
    virtual Color3f
    F(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const { return {0.f}; }

    virtual Color3f
    Eval(const BxDFSampleRecord &bRec) const { return {0.f}; }

    virtual Float
    Pdf(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const { return 0.f; };

    virtual Float
    pdf(const BxDFSampleRecord &bRec) const { return 0.f; };

    virtual std::optional<Color3f>
    SampleF(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const = 0;

    // Sample bxdf & return pdf
    virtual Color3f
    Sample(BxDFSampleRecord &bRec, Float &pdf, const Point2f &sample) const { return {0.f}; };

    virtual BxDFFlag
    Flag() const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EBxDF;
    }
};

NAMESPACE_END
