//
// Created by 44772 on 2024/2/4.
//

#include "base/shape.h"
#include "base/mesh.h"

NAMESPACE_BEGIN

    class Triangle : public Shape {
    public:
        explicit Triangle(Mesh *mesh, Integer index) : pMesh(mesh) {
            for(Integer i = 0; i < 3; i++) {
                mIndices[i] = index + i;
            }
        }

        Boolean Intersect(const Ray &ray, Float tMax, Interaction &i) const override {

            return false;
        }

        Bound3f BoundingBox() const override {
            return {};
        }

    private:
        Mesh *pMesh{nullptr};
        Integer mIndices[3];
    };

NAMESPACE_END