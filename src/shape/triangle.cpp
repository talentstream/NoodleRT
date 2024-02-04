//
// Created by 44772 on 2024/2/4.
//

#include "base/shape.h"
#include "base/mesh.h"

NAMESPACE_BEGIN

    class Triangle : public Shape {
    public:
        explicit Triangle(Mesh *mesh, Integer index) : pMesh(mesh) {
            for (Integer i = 0; i < 3; i++) {
                mIndices[i] = index + i;
            }
        }

        Boolean Intersect(const Ray &ray, Float tMax, Interaction &i) const override {
            const Point3f &v0 = pMesh->positions[mIndices[0]];
            const Point3f &v1 = pMesh->positions[mIndices[1]];
            const Point3f &v2 = pMesh->positions[mIndices[2]];

            // en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
            Vector3f edge1 = v1 - v0;
            Vector3f edge2 = v2 - v0;
            Vector3f rayCrossEdge2 = Cross(ray.d, edge2);
            Float det = Dot(edge1, rayCrossEdge2);
            if (Abs(det) < Epsilon) {
                return false;
            }

            Float invDet = 1 / det;
            Vector3f s = ray.o - v0;
            Float u = Dot(s, rayCrossEdge2) * invDet;
            if (u < 0 || u > 1) {
                return false;
            }

            Vector3f sCrossEdge1 = Cross(s, edge1);
            Float v = Dot(ray.d, sCrossEdge1) * invDet;
            if (v < 0 || u + v > 1) {
                return false;
            }

            Float t = Dot(edge2, sCrossEdge1) * invDet;
            if(t > Epsilon)
            {
                i.t = t;
                i.p = ray(t);
                Normal3f outNormal = Normalize(Normal3f{Cross(edge1, edge2)});
                i.front = Dot(ray.d, Vector3f{i.n}) < 0;
                i.n = i.front ? outNormal : -outNormal;
                return true;
            }

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