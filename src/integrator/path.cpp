//
// Created by talentstream on 2024/2/25.
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

class PathIntegrator : public ImageTileIntegrator {
public:
    explicit PathIntegrator(const PropertyList &propertyList)
            : ImageTileIntegrator(propertyList) {
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
            L += beta * iRec.Le(-ray.d);

            // End path if max depth is reached
            if (depth++ == mMaxDepth) break;

            auto bxdf = iRec.bxdf;
            if (!bxdf) break;

            // Sample direct illumination
//            {
//                Vector3f wo = -ray.d;
//                for (const auto light: mLights) {
//                    if(IsInfiniteLight(light->Flag())){
//                        continue;
//                    }
//                    LightSampleRecord lRec{si,pSampler};
//                    Color3f li = light->Sample_Li(lRec);
//                    if (li.IsZero() || lRec.pdf == 0) {
//                        continue;
//                    }
//
//                    Ray lr {si.p, lRec.wi, 0};
//                    BxDFSampleRecord bRec{si, pSampler, si.shading.ToLocal(lRec.wi),si.shading.ToLocal(Normalize(-ray.d))};
//                    if(!pAggregate->UnOccluded(lr)) {
//                        Vector3f LDir = Normalize(lRec.wi);
//                        Float LoN = Max(0.f,Dot(LDir, si.shading.n));
//
//                        L += beta * bxdf->Eval(bRec) * li * LoN / lRec.pdf;
//
//                    }
//                }
//            }
            // Sample outgoing direction at intersection to continue path
            {

                BxDFRecord bRec{iRec.ToLocal(-ray.d)};
                bRec.uv = iRec.uv;
                Float bxdfPdf;
                Color3f bxdfValue = bxdf->Sample(bRec, bxdfPdf, pSampler->Next2D());
                if (bxdfValue.IsZero() || bxdfPdf < Epsilon) {
                    break;
                }

                beta *= bxdfValue;
                ray = iRec.GenerateRay(iRec.ToWorld(bRec.wo));
            }

            // RR
            if (depth > 3) {
                Float q = Max(0.f, 1 - MaxValue(beta));
                if (pSampler->Next1D() < q) {
                    break;
                }
                beta /= (1 - q);
            }
        }

        return L;
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(PathIntegrator, "path")

NAMESPACE_END