//
// Created by talentstream on 2024/2/25.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "base/sampler.h"
#include "core/interaction.h"
#include "util/sampling.h"
#include <print>

NAMESPACE_BEGIN

class PathIntegrator : public ImageTileIntegrator {
public:
    explicit PathIntegrator(const PropertyList &propertyList) {
        mSpp = propertyList.GetInteger("spp", 1);
        mMaxDepth = propertyList.GetInteger("depth", 1);
        PRINT_DEBUG_INFO("Integrator", "path")
    }

    [[nodiscard]] Color3f Li(const Ray &r) const override {
        // Todo: has bug
        Ray ray = r;
        Color3f L{0}, beta(1.0f);
        Integer depth{0};
        while (beta != Color3f{0}) {
            // Intersect ray with scene
            SurfaceInteraction si;
            // Account for infinite lights if ray has no intersection
            if (!pAggregate->Intersect(ray, si)) {
                for (const auto light: mLights) {
                    if (!IsInfiniteLight(light->Flag())) {
                        continue;
                    }
                    L += beta * light->Le(ray);
                }
                break;
            }

            // Add emitted light at intersection
            L += beta * si.Le(-ray.d);

            // End path if max depth is reached
            if (depth++ == mMaxDepth) {
                break;
            }

            auto bxdf = si.bxdf;
            if (!bxdf) {
                //
                break;
            }

            // Sample direct illumination
            {
                Vector3f wo = -ray.d;
                for (const auto light: mLights) {
                    if(IsInfiniteLight(light->Flag())){
                        continue;
                    }
                    LightSampleRecord lRec{si,pSampler};
                    Color3f li = light->Sample_Li(lRec);
                    if (li.IsZero() || lRec.pdf == 0) {
                        continue;
                    }

                    Ray lr {si.p, lRec.wi, 0};
                    BxDFSampleRecord bRec{si, pSampler, si.shading.ToLocal(lRec.wi),si.shading.ToLocal(Normalize(-ray.d))};
                    if(!pAggregate->UnOccluded(lr)) {
                        Vector3f LDir = Normalize(lRec.wi);
                        Float LoN = Max(0.f,Dot(LDir, si.shading.n));

                        L += beta * bxdf->Eval(bRec) * li * LoN / lRec.pdf;

                    }
                }
            }
            // Sample outgoing direction at intersection to continue path
            {
                Float bsdfPdf;
                BxDFSampleRecord bRec{si, pSampler, si.shading.ToLocal(si.wi)};
                Color3f bxdfWeight = bxdf->Sample(bRec, bsdfPdf, pSampler->Next2D());
                if (bxdfWeight.IsZero()) {
                    break;
                }

                auto f = bxdf->Eval(bRec);
                auto pdf = bxdf->pdf(bRec);

                if (!pdf) {
                    break;
                }

                beta *= bxdfWeight / pdf;

                ray = si.GenerateRay(bRec.wo);
            }
        }
        return L;
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(PathIntegrator, "path")

NAMESPACE_END