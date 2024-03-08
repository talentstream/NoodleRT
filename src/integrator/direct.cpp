//
// Created by talentstream on 2024/3/3.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bsdf.h"
#include "base/emitter.h"
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
            if (iRec.emitter) break;
            EmitterRecord eRec{iRec.p};

            Color3f li = light->Sample(eRec, pSampler->Next2D());
            if (li.IsZero() || eRec.pdf == 0.f) continue;

            BxDFRecord bRec{iRec.ToLocal(eRec.wi), iRec.ToLocal(-ray.d)};
            bRec.uv = iRec.uv;
            Color3f bxdfVal = bxdf->Eval(bRec);
            Float bxdfPdf = bxdf->Pdf(bRec);
            if (bxdfPdf == 0.f) continue;

            Float tMax = Length(eRec.p - iRec.p);
            if (pAggregate->UnOccluded(iRec.GenerateRay(eRec.wi), tMax)) {
                Float weight = Weight(1, eRec.pdf, 1, bxdfPdf);
                L += bxdfVal * li * weight / eRec.pdf;
            }
        }

        return L;
    }

    inline Float Weight(Integer nA, Float pdfA, Integer nB, Float pdfB) const {
        Float A = nA * pdfA;
        Float B = nB * pdfB;
        return (A * A) / (A * A + B * B);
    }

private:
};

REGISTER_CLASS(Direct, "direct")

NAMESPACE_END