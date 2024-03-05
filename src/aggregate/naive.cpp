//
// Created by 44772 on 2024/1/31.
//

#include "base/aggregate.h"
#include "base/primitive.h"
#include <vector>

NAMESPACE_BEGIN

class Naive : public aggregate {
public:
    explicit Naive(const PropertyList &propertyList) {
        mShapeOffset.push_back(0u);

        PRINT_DEBUG_INFO("aggregate", "naive")
    }

    void
    Build() override {
        auto size = GetPrimitiveCount();
        mShapeIndices.resize(size);
        for (auto i{0}; i < size; i++) {
            mShapeIndices[i] = i;
        }
    }

    Boolean
    Intersect(const Ray &ray, IntersectionRecord &si) const override {
        Boolean hitAnything{false};
        IntersectionRecord tempSi;
        for (auto i{0}; i < mShapeIndices.size(); i++) {
            auto idx = mShapeIndices[i];
            const auto s = mShapes[FindShape(idx)];
            if (s->Intersect(idx, ray, si.t, tempSi)) {
                hitAnything = true;
                if (tempSi.t < si.t) {
                    si = tempSi;
                }
            }
        }

        return hitAnything;
    }

    Boolean
    UnOccluded(const Ray &ray) const override {
        for (auto i{0}; i < mShapeIndices.size(); i++) {
            auto idx = mShapeIndices[i];
            const auto s = mShapes[FindShape(idx)];
            if (s->IntersectP(idx, ray)) {
                return false;
            }
        }
        return true;
    }
};

REGISTER_CLASS(Naive, "naive")

NAMESPACE_END