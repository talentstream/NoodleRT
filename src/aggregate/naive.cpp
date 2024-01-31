//
// Created by 44772 on 2024/1/31.
//

#include "base/aggregate.h"
#include "base/primitive.h"
#include <vector>
#include <print>

NAMESPACE_BEGIN

    class NaiveAggregate : public Aggregate {
    public:
        explicit NaiveAggregate(const PropertyList &propertyList) {
            std::print("->NaiveAggregate\n");
        }

        void AddChild(Object *object) {
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
            Float closest{Infinity};
            Boolean hitAnything{false};
            Interaction tempInteraction;

            for (const auto &primitive: mPrimitives) {
                if (primitive->Intersect(ray, tempInteraction)) {
                    hitAnything = true;
                    if (tempInteraction.t < closest) {
                        closest = tempInteraction.t;
                        interaction = tempInteraction;
                    }
                }
            }

            return hitAnything;


        }


    private:
        std::vector<Primitive *> mPrimitives;
    };

    REGISTER_CLASS(NaiveAggregate, "naive")

NAMESPACE_END