//
// Created by talentstream on 2024/2/25.
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

class PathIntegrator : public ImageTileIntegrator {
public:
    explicit PathIntegrator(const PropertyList &propertyList)
            : ImageTileIntegrator(propertyList) {
        mMaxDepth = propertyList.GetInteger("depth", 1);
        PRINT_DEBUG_INFO("Integrator", "path")
    }

    [[nodiscard]] Color3f Li(const Ray &r) const override {
        // has bug
        Ray ray = r;
        Color3f L{0}, beta(1.0f);
        Integer depth{0};
        while (beta != Color3f{0}) {
            // Intersect ray with scene
            IntersectionRecord iRec;
            // Account for infinite lights if ray has no intersection
            if (!pAggregate->Intersect(ray, iRec)) {
                for (const auto light: mLights) {
                    if (IsInfiniteLight(light->Flag())) {
                        L += beta * light->Le(ray);
                    }
                }
                break;
            }

            // Add emitted light at intersection
            if (iRec.emitter) {
                L += beta * iRec.Le(-ray.d);
            }

            // End path if max depth is reached
            if (depth++ == mMaxDepth) break;

            auto bxdf = iRec.bxdf;
            if (!bxdf) break;

            // Sample direct illumination
            {
                for (const auto light: mLights) {
                    if (iRec.emitter) break;
                    EmitterRecord eRec{iRec.p};

                    Color3f li = light->Sample(eRec, pSampler->Next2D());
                    if (li.IsZero() || eRec.pdf == 0) continue;
                    Float tMax = Length(eRec.p - eRec.ref) - Epsilon;
                    if (!pAggregate->UnOccluded(iRec.GenerateRay(eRec.wi), tMax)) continue;
                    BxDFRecord bRec{iRec.ToLocal(-ray.d), iRec.ToLocal(eRec.wi)};
                    bRec.uv = iRec.uv;
                    Color3f bxdfVal = bxdf->Eval(bRec);
                    Float bxdfPdf = bxdf->Pdf(bRec);
                    if (bxdfPdf == 0.f || bxdfPdf == 1.f) continue;
                    Float weight = Weight(1, eRec.pdf, 1, bxdfPdf);
                    L += beta * bxdfVal * li * weight;
                }
            }

            // RR
            if (depth > 3 && !iRec.emitter) {
                Float q = Max(0.f, 1 - MaxValue(beta));
                if (pSampler->Next1D() < q) {
                    break;
                }
                beta /= (1 - q);
            }

            // Sample outgoing direction at intersection to continue path
            {

                BxDFRecord bRec{iRec.ToLocal(-ray.d)};
                bRec.uv = iRec.uv;
                Float bxdfPdf;
                Color3f bxdfValue = bxdf->Sample(bRec, bxdfPdf, pSampler->Next2D());
                if (bxdfValue.IsZero() || bxdfPdf == 0.f) {
                    break;
                }
                beta *= bxdfValue;

                ray = iRec.GenerateRay(iRec.ToWorld(bRec.wo));
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
    Integer mMaxDepth;
};

REGISTER_CLASS(PathIntegrator, "path")

NAMESPACE_END