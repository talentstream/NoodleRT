//
// Created by 44772 on 2024/1/31.
//

#include "base/aggregate.h"
#include <vector>

NAMESPACE_BEGIN

class Naive : public Aggregate {
public:
    explicit Naive(const PropertyList &propertyList) {
        mShapeOffset.push_back(0u);

        PRINT_DEBUG_INFO("aggregate", "naive")
    }

    void
    Build() override {
        auto size = GetPrimitiveCount();
        if (size == 0) return;
        mShapeIndices.resize(size);
        for (auto i{0}; i < size; i++) {
            mShapeIndices[i] = i;
        }
    }

    Boolean
    Intersect(const Ray &ray, IntersectionRecord &iRec) const override {
        Boolean findIntersection{false};
        for (auto i{0}; i < mShapeIndices.size(); i++) {
            auto idx = mShapeIndices[i];
            const auto s = mShapes[FindShape(idx)];
            if (s->Intersect(idx, ray, iRec)) {
                findIntersection = true;
            }
        }

        return findIntersection;
    }

    Boolean
    UnOccluded(const Ray &ray, Float tMax) const override {
        for (auto i{0}; i < mShapeIndices.size(); i++) {
            auto idx = mShapeIndices[i];
            const auto s = mShapes[FindShape(idx)];
            if (s->IsEmitter()) return true;
            if (s->IntersectP(idx, ray, tMax)) return false;
        }

        return true;
    }
};

REGISTER_CLASS(Naive, "naive")

NAMESPACE_END