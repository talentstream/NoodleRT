//
// Created by 44772 on 2024/1/31.
//

#include "base/primitive.h"
#include "base/shape.h"
#include "base/bxdf.h"
#include "base/light.h"
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

    Boolean
    Intersect(const Ray &ray, IntersectionRecord &si) const override {
        if (!pShape->Intersect(ray, si.t, si)) {
            return false;
        }

        si.bxdf = pBxDF;
        si.emitter = pLight;
        return true;
    }

    Boolean
    IntersectP(const Ray &ray) const override {
        return pShape->IntersectP(ray, Infinity);
    }

    Bound3f
    BoundingBox() const override {
        return pShape->BoundingBox();
    }

    void AddChild(Object *child) override {

        switch (child->GetClassType()) {
            case EClassType::EShape:
                pShape = dynamic_cast<Shape *>(child);
                break;
            case EClassType::EBxDF:
                pBxDF = dynamic_cast<BxDF *>(child);
                break;
            case EClassType::ELight:
                pLight = dynamic_cast<Emitter *>(child);
                break;
        }
    }

    void Initialize() override {
        if (pBxDF == nullptr) {
            pBxDF = dynamic_cast<BxDF *>(ObjectFactory::CreateInstance("diffuse", PropertyList(), true));
        }
    }

private:
    Shape *pShape;
    BxDF *pBxDF{nullptr};
    Emitter *pLight{nullptr};
};

REGISTER_CLASS(GeometryPrimitive, "geometry")

NAMESPACE_END