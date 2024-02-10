//
// Created by 44772 on 2024/1/31.
//

#include "base/primitive.h"
#include "base/shape.h"
#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

class GeometryPrimitive : public Primitive {
public:
    explicit GeometryPrimitive(const PropertyList &propertyList) {
        PRINT_DEBUG_INFO("Primitive", "geometry")
    }

    ~GeometryPrimitive() {
        delete pShape;
        delete pBxDF;
    }

    void AddChild(Object *object) override {
        switch (object->GetClassType()) {
            case EClassType::EShape:
                pShape = dynamic_cast<Shape *>(object);
                break;
            case EClassType::EBxDF:
                pBxDF = dynamic_cast<BxDF *>(object);
                break;
        }
    }

    void Initialize() override {
        if (!pBxDF) {
            pBxDF = dynamic_cast<BxDF *>(ObjectFactory::CreateInstance("lambert", PropertyList(), true));
        }
    }

    Boolean Intersect(const Ray &ray, Interaction &interaction) const override {
        if (!pShape->Intersect(ray, interaction.t, interaction)) {
            return false;
        }

        interaction.bxdf = pBxDF;

        return true;
    }

    Bound3f BoundingBox() const override {
        return pShape->BoundingBox();
    }

private:
    Shape *pShape;
    BxDF *pBxDF{nullptr};
};

REGISTER_CLASS(GeometryPrimitive, "geometry")

NAMESPACE_END