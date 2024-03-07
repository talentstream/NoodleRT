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
    explicit WhittedIntegrator(const PropertyList &propertyList)
            : ImageTileIntegrator(propertyList) {
        mMaxDepth = propertyList.GetInteger("depth", 1);
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
        if (iRec.emitter) {
            L += iRec.Le(-ray.d);
        }

        auto bxdf = iRec.bxdf;
        if (!bxdf) return L;

        // if diffuse calculate direct illumination
        if (IsDiffuse(bxdf->Flag())) {
            for (const auto light: mLights) {
                if (iRec.emitter) break;
                EmitterRecord eRec{iRec.p};

                Color3f li = light->SampleLi(eRec, pSampler->Next2D());

                Float tMax = Length(eRec.p - eRec.ref) - Epsilon;
                if (!pAggregate->UnOccluded(iRec.GenerateRay(eRec.wi), tMax)) {
                    return 0.f;
                };
                if (li.IsZero() || eRec.pdf == 0.f) continue;

                BxDFRecord bRec{iRec.ToLocal(eRec.wi), iRec.ToLocal(-ray.d)};
                bRec.uv = iRec.uv;
                Color3f bxdfVal = bxdf->Eval(bRec);

                L += bxdfVal * li;
            }
            return L;
        } else {
            // if not diffuse, indirect
            BxDFRecord bRec{iRec.ToLocal(-ray.d)};
            bRec.uv = iRec.uv;
            Float pdf;
            auto bxdfValue = bxdf->Sample(bRec, pdf, pSampler->Next2D());
            if (depth + 1 < mMaxDepth) {
                return L + bxdfValue * Trace(iRec.GenerateRay(iRec.ToWorld(bRec.wo)), depth + 1);
            } else return {0.f};
        }
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END