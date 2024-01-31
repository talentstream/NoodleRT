//
// Created by 44772 on 2024/1/31.
//

#include "base/primitive.h"
#include <print>

NAMESPACE_BEGIN

    class GeometryPrimitive : public Primitive {
    public:
        explicit GeometryPrimitive(const PropertyList &propertyList) {
            PRINT_DEBUG_INFO("Primitive", "geometry")
        }

        ~GeometryPrimitive()
        {
            delete pShape;
        }

        void AddChild(Object *object) override {
            switch (object->GetClassType()) {
                case EClassType::EShape:
                    pShape = static_cast<Shape *>(object);
                    break;
            }
        }

        Boolean Intersect(const Ray &ray, Interaction &interaction) const override {
            return pShape->Intersect(ray, Infinity, interaction);
        }

    private:
        Shape *pShape;

    };

    REGISTER_CLASS(GeometryPrimitive,"geometry")

NAMESPACE_END