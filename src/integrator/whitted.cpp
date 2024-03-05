//
// Created by 44772 on 2024/1/31.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "core/record.h"
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
        Color3f L{0.f};

        // find nearest intersection
        IntersectionRecord iRec;
        if (!pAggregate->Intersect(ray, iRec)) {
            for (const auto light: mLights) {
                if (IsInfiniteLight(light->Flag())) {
                    L += light->Le(ray);
                }
            }
            return L;
        }

        // emitter
        L += iRec.Le(-ray.d);

        auto bxdf = iRec.bxdf;
        if (!bxdf) return {0};

        BxDFRecord bRec{iRec.ToLocal(-ray.d)};
        bRec.uv = iRec.uv;
        Float pdf;
        auto bxdfValue = bxdf->Sample(bRec, pdf, pSampler->Next2D());

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
                L += bxdfValue;
            } else {
                L += bxdfValue * Trace(iRec.GenerateRay(iRec.ToWorld(bRec.wo)), depth + 1);
            }
        }
        return L;
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END