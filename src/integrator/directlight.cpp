//
// Created by talentstream on 2024/3/3.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "base/sampler.h"
#include "core/record.h"
#include "util/sampling.h"

#include <print>

NAMESPACE_BEGIN

// just direct light for debug
class Direct : public ImageTileIntegrator {
public:
    explicit Direct(const PropertyList &propertyList)
            : ImageTileIntegrator(propertyList) {
        PRINT_DEBUG_INFO("Integrator", "directlight")
    }

    Color3f
    Li(const Ray &ray) const override {
        Color3f L{0};

        IntersectionRecord iRec;
        // if no intersection, add infinite light
        if (!pAggregate->Intersect(ray, iRec)) {
            for (const auto light: mLights) {
                if (IsInfiniteLight(light->Flag())) {
                    L += light->Le(ray);
                }
            }
            return L;
        }

        auto bxdf = iRec.bxdf;
        if (!bxdf) return L;

        // Add emitted light at intersection
        L += iRec.Le(-ray.d);

        for (const auto light: mLights) {
            if (IsInfiniteLight(light->Flag())) {
                continue;
            }

            EmitterRecord eRec{iRec.p};

            Color3f li = light->Sample_Li(eRec,pSampler->Next2D());
            if (li.IsZero() || eRec.pdf < Epsilon) continue;

            BxDFRecord bRec{iRec.ToLocal(eRec.wi), iRec.ToLocal(-ray.d)};
            Color3f bxdfVal = bxdf->Eval(bRec);
            Float bxdfPdf = bxdf->pdf(bRec);
            if (bxdfPdf == 0.f || eRec.pdf == 0.f) continue;

            if(pAggregate->UnOccluded(iRec.GenerateRay(eRec.wi))) {
                Float weight = Weight(bxdfPdf, eRec.pdf);

                L += bxdfVal * li * weight;
            }
        }

        return L;
    }

    inline Float Weight(Float pdfA, Float pdfB) const {
        pdfA *= pdfA;
        pdfB *= pdfB;
        return pdfA / (pdfA + pdfB);
    }

private:
};

REGISTER_CLASS(Direct, "direct")

NAMESPACE_END