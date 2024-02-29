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
        Ray ray = r;
        Color3f L{0}, beta(1.0f);
        Integer depth{0};
        while (beta != Color3f{0}) {
            // Intersect ray with scene
            SurfaceInteraction si;
            // Account for infinite lights if ray has no intersection
            if (!pAggregate->Intersect(ray, si)) {
                for (const auto light: mLights) {
                    L += beta * light->Le(ray);
                }
                break;
            }

            // End path if max depth is reached
            if (depth++ == mMaxDepth) {
                break;
            }

            auto bxdf = si.bxdf;
            if (!bxdf) {
                //
                continue;
            }

            // Sample direct illumination
            {
                Vector3f wo = -ray.d;
                for (const auto light: mLights) {
                    Point2f lightSample = pSampler->Next2D();
                    Vector3f wi;
                    Color3f Li = light->SampleLi(si, wi, lightSample);
                    Ray lightRay{si.p, wi, si.t};
                    SurfaceInteraction lightSi;
                    if (!pAggregate->Intersect(lightRay, lightSi)) {
                        L += beta * bxdf->F(si, wo, wi) * Li * AbsDot(wi, si.shading.n);
                    }
                }
            }
            // Sample outgoing direction at intersection to continue path
            {
                Point2f bxdfSample = pSampler->Next2D();
                Vector3f wo = si.shading.ToLocal(-ray.d), wi;
                auto sampleF = bxdf->SampleF(si, wo, wi, bxdfSample);
                if (!sampleF.has_value()) {
                    break;
                }
                auto f = bxdf->F(si, wo, wi);
                auto pdf = bxdf->Pdf(si, wo, wi);
                if(!pdf) {
                    break;
                }
                beta *= f * Abs(Frame::CosTheta(wi)) / pdf;
                ray = si.GenerateRay(wi);
            }
        }
        return L;
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(PathIntegrator, "path")

NAMESPACE_END