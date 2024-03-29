//
// Created by 44772 on 2024/2/4.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include <print>

NAMESPACE_BEGIN

class NormalIntegrator : public ImageTileIntegrator {
public:
    explicit NormalIntegrator(const PropertyList &propertyList) : ImageTileIntegrator(propertyList) {
        mSpp = propertyList.GetInteger("spp", 1);
        PRINT_DEBUG_INFO("Integrator", "normal")
    }

    [[nodiscard]] Color3f Li(const Ray &ray) const override {

        IntersectionRecord si;
        if (!pAggregate->Intersect(ray, si)) {
            return {};
        }

        auto n = si.n;
        return Color3f{n.x + 1, n.y + 1, n.z + 1} * 0.5f;
    }

};

REGISTER_CLASS(NormalIntegrator, "normal")

NAMESPACE_END