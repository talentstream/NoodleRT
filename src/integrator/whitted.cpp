//
// Created by 44772 on 2024/1/31.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "core/interaction.h"
#include "base/sampler.h"

#include <print>

NAMESPACE_BEGIN

class WhittedIntegrator : public ImageTileIntegrator {
public:
    explicit WhittedIntegrator(const PropertyList &propertyList) {
        mMaxDepth = propertyList.GetInteger("depth", 1);
        mSpp = propertyList.GetInteger("spp", 1);
        PRINT_DEBUG_INFO("Integrator", "whitted")
    }

    Color3f Li(const Ray &ray) const override {
        return Trace(ray, 0);
    }

private:
    Color3f Trace(const Ray &ray, Integer depth) const {
        // find nearest intersection
        IntersectionRecord si;
        Color3f L{0};
        if (!pAggregate->Intersect(ray, si)) {
            for (const auto light: mLights) {
                if (IsInfiniteLight(light->Flag())) {
                    L += light->Le(ray);
                }

            }
            return L;
        }

        // shading
        auto bxdf = si.bxdf;
        if (!bxdf) return {0};
        Vector3f wi = -ray.d;

        // emitted color
        Color3f emitted;// = si.Le(wi);

        BxDFSampleRecord bRec{si, pSampler, si.shading.ToLocal(wi)};
        Float pdf;
        auto bxdfValue = bxdf->Sample(bRec, pdf, pSampler->Next2D());

        auto le = bxdfValue;

        // calculate direct illumination

//        for (const auto light: mLights) {
//            Point2f lightSample{0.5f, 0.5f};
//            Vector3f w;// direction to light
//            Color3f li = light->SampleLi(si, w, lightSample);
//            Ray lightRay{si.p, w, si.t};
//            SurfaceInteraction lightSi;
//            if (!pAggregate->Intersect(lightRay, lightSi)) {
//                emitted = li * le * Abs(Dot(w, si.n));
//            }
//        }

        // calculate indirect illumination
        if (depth + 1 < mMaxDepth) {
            if (IsDiffuse(bxdf->Flag())) {
                return emitted + le;
            } else {
                return emitted + le * Trace(si.GenerateRay(si.shading.ToWorld(bRec.wo)), depth + 1);
            }
        }
        return L;
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END