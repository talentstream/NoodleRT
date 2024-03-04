//
// Created by talentstream on 2024/3/4.
//

#include "base/mesh.h"
#include "base/primitive.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "triangle.h"
#include <print>

NAMESPACE_BEGIN

class Rectangle final : public Mesh {
public:
    explicit Rectangle(const PropertyList &propertyList)
            : Mesh(propertyList) {
        PRINT_DEBUG_INFO("Rectangle", "rectangle")
    }

    void Initialize() override {
        positions = {{-1, 0, -1},
                     {1,  0, -1},
                     {1,  0, 1},
                     {1,  0, 1},
                     {-1, 0, 1},
                     {-1, 0, -1}};
        Normal3f n = {0, 1, 0};
        normals = {n, n, n, n, n, n};
        uvs = {{0, 0},
               {1, 0},
               {1, 1},
               {1, 1},
               {0, 1},
               {0, 0}};

        auto *triangle0 = new Triangle(this, 0);
        auto primitive0 = dynamic_cast<Primitive *>(ObjectFactory::CreateInstance("geometry", PropertyList()));
        primitive0->AddChild(triangle0);

        if (pBxDF == nullptr) {
            pBxDF = dynamic_cast<BxDF *>(ObjectFactory::CreateInstance("diffuse", PropertyList(), true));
        }
        primitive0->AddChild(pBxDF);

        if (pLight) {
            primitive0->AddChild(pLight);
        }

        primitives.push_back(primitive0);

        auto *triangle1 = new Triangle(this, 3);
        auto primitive1 = dynamic_cast<Primitive *>(ObjectFactory::CreateInstance("geometry", PropertyList()));
        primitive1->AddChild(triangle1);
        primitive1->AddChild(pBxDF);
        if (pLight) {
            primitive1->AddChild(pLight);
        }
        primitives.push_back(primitive1);
    }
};

REGISTER_CLASS(Rectangle, "rectangle")

NAMESPACE_END