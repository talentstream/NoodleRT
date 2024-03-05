//
// Created by talentstream on 2024/3/3.
//

#include "base/integrator.h"
#include "base/Aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "base/sampler.h"
#include "core/record.h"
#include "util/sampling.h"

#include <print>

NAMESPACE_BEGIN

// just direct light for debug
class DirectLightIntegrator : public ImageTileIntegrator {
public:
    explicit DirectLightIntegrator(const PropertyList &propertyList) {
        mSpp = propertyList.GetInteger("spp", 1);
        PRINT_DEBUG_INFO("Integrator", "directlight")
    }

    [[nodiscard]] Color3f Li(const Ray &ray) const override {
//        Color3f L{0.f};
//        // Intersect ray with scene
//        SurfaceInteraction si;
//        // Account for infinite lights if ray has no intersection
//        if (!pAggregate->Intersect(ray, si)) {
//            for (const auto light: mLights) {
//                if (!IsInfiniteLight(light->Flag())) {
//                    continue;
//                }
//                L += light->Le(ray);
//            }
//            return L;
//        }
//
//        auto bxdf = si.bxdf;
//        if (!bxdf) {
//            return L;
//        }
//
//        Vector3f wi = si.wi;
//        for (const auto light: mLights) {
//            if (IsInfiniteLight(light->Flag())) {
//                continue;
//            }
//
//            LightSampleRecord lRec{si, pSampler};
//            Color3f Li = light->Sample_Li(lRec);
//            if (Li.IsZero() || lRec.pdf == 0) {
//                continue;
//            }
//            BxDFSampleRecord bRec{si, pSampler, si.shading.ToLocal(lRec.wi), si.shading.ToLocal(si.wi)};
//            Float bxdfPdf;
//            Color3f bxdfVal = bxdf->Sample(bRec, bxdfPdf, pSampler->Next2D());
//            if (!pAggregate->UnOccluded(si.GenerateRay(-lRec.wi))) {
//                Float weight = Weight(bxdfPdf, lRec.pdf);
//                L += bxdfVal * Li * weight;
//            }
//        }
//        return L;
        return {};
    }

    inline Float Weight(Float pdfA, Float pdfB) const {
        pdfA *= pdfA;
        pdfB *= pdfB;
        return pdfA / (pdfA + pdfB);
    }

private:
};

REGISTER_CLASS(DirectLightIntegrator, "directlight")

NAMESPACE_END