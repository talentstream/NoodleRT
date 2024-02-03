//
// Created by 44772 on 2024/2/3.
//

#include "base/aggregate.h"
#include "base/primitive.h"
#include <print>
#include <vector>

NAMESPACE_BEGIN

    class BVHAggregate : public Aggregate {
    public:
        explicit BVHAggregate(const PropertyList &propertyList) {
            PRINT_DEBUG_INFO("Aggregate", "bvh")
        }

        void AddChild(Object *object) override {
            switch (object->GetClassType()) {
                case EClassType::EPrimitive:
                    mPrimitives.emplace_back(static_cast<Primitive *>(object));
                    break;
                default:
                    /*throw*/
                    break;
            }
        }

        Boolean Intersect(const Ray &ray, Interaction &interaction) const override {
            return false;
        }

    private:
        std::vector<Primitive *> mPrimitives;
    };

NAMESPACE_END
