//
// Created by 44772 on 2024/1/31.
//

#include "base/aggregate.h"
#include "base/primitive.h"
#include "base/mesh.h"
#include <vector>
#include <ranges>
#include <print>

NAMESPACE_BEGIN

class NaiveAggregate : public Aggregate {
public:
    explicit NaiveAggregate(const PropertyList &propertyList) {
        PRINT_DEBUG_INFO("Aggregate", "naive")
    }

    void AddChild(Object *object) {
        switch (object->GetClassType()) {
            case EClassType::EPrimitive:
                mPrimitives.emplace_back(dynamic_cast<Primitive *>(object));
                break;
            case EClassType::EMesh: {
                auto mesh = dynamic_cast<Mesh *>(object);
                mPrimitives.insert(mPrimitives.end(), mesh->primitives.begin(), mesh->primitives.end());
                break;
            }
            default:
                /*throw*/
                break;
        }
    }

    Boolean Intersect(const Ray &ray, SurfaceInteraction &si) const override {
        Boolean hitAnything{false};
        SurfaceInteraction tempSi;

        // Todo: naive has bug
        for (const auto &primitive: mPrimitives) {
            if (primitive->Intersect(ray, tempSi)) {
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
        return std::ranges::all_of(mPrimitives, [&ray](const auto &primitive) {
            return !primitive->IntersectP(ray);
        });
    }

private:
    std::vector<Primitive *> mPrimitives;
};

class naive : public aggregate {
public:
    explicit naive(const PropertyList &propertyList) {
        PRINT_DEBUG_INFO("Aggregate", "naive")
    }

    void
    Build() override {
        auto size = GetPrimitiveCount();
        mShapeIndices.resize(size);
        for(auto i{0}; i < size; i++) {
            mShapeIndices[i] = i;
        }
    }

    Boolean
    Intersect(const Ray &ray, SurfaceInteraction &si) const override {
        Boolean hitAnything{false};
        SurfaceInteraction tempSi;
        for(auto i{0}; i < mShapeIndices.size(); i++) {
            auto idx = mShapeIndices[i];
            const auto s = mShapes[FindShape(idx)];
            if (s->Intersect(idx, ray, si.t,tempSi)) {
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

REGISTER_CLASS(NaiveAggregate, "naive")

NAMESPACE_END