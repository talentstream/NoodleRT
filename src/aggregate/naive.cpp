//
// Created by 44772 on 2024/1/31.
//

#include "base/aggregate.h"
#include "base/primitive.h"
#include "base/mesh.h"
#include <vector>
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

        Boolean Intersect(const Ray &ray, Interaction &interaction) const override {
            Boolean hitAnything{false};
            Interaction tempInteraction;

            for (const auto &primitive: mPrimitives) {
                if (primitive->Intersect(ray, tempInteraction)) {
                    hitAnything = true;
                    if (tempInteraction.t < interaction.t) {
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