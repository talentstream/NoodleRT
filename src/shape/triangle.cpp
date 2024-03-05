//
// Created by 44772 on 2024/2/4.
//

#include "triangle.h"
#include <print>

NAMESPACE_BEGIN

//Triangle::Triangle(Mesh *mesh, Integer index)
//        : pMesh{mesh} {
//    for (Integer i = 0; i < 3; i++) {
//        mIndices[i] = index + i;
//    }
//
//}
//
//Boolean Triangle::Intersect(const Ray &ray, Float tMax, IntersectionRecord &si) const {
//    const Point3f &v0 = pMesh->positions[mIndices[0]];
//    const Point3f &v1 = pMesh->positions[mIndices[1]];
//    const Point3f &v2 = pMesh->positions[mIndices[2]];
//
//    // en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
//    Vector3f edge1 = v1 - v0;
//    Vector3f edge2 = v2 - v0;
//    Vector3f rayCrossEdge2 = Cross(ray.d, edge2);
//    Float det = Dot(edge1, rayCrossEdge2);
//    if (det < Epsilon) {
//        return false;
//    }
//
//    Float invDet = 1 / det;
//    Vector3f s = ray.o - v0;
//    Float u = Dot(s, rayCrossEdge2) * invDet;
//    if (u < 0 || u > 1) {
//        return false;
//    }
//
//    Vector3f sCrossEdge1 = Cross(s, edge1);
//    Float v = Dot(ray.d, sCrossEdge1) * invDet;
//    if (v < 0 || u + v > 1) {
//        return false;
//    }
//
//    Float t = Dot(edge2, sCrossEdge1) * invDet;
//
//    if (t > Epsilon) {
//        Normal3f outNormal;
//        if (pMesh->normals.size() == 0) {
//            outNormal = Normalize(Normal3f{Cross(edge1, edge2)});
//
//        } else {
//            const Normal3f &n1 = pMesh->normals[mIndices[0]];
//            const Normal3f &n2 = pMesh->normals[mIndices[1]];
//            const Normal3f &n3 = pMesh->normals[mIndices[2]];
//            outNormal = Normalize((1 - u - v) * n1 + u * n2 + v * n3);
//        }
//
//        si = IntersectionRecord(t, ray(t), outNormal);
//
//        // uv
//        if (!pMesh->uvs.size() == 0) {
//            const Point2f &uv1 = pMesh->uvs[mIndices[0]];
//            const Point2f &uv2 = pMesh->uvs[mIndices[1]];
//            const Point2f &uv3 = pMesh->uvs[mIndices[2]];
//            si.uv = (1 - u - v) * uv1 + u * uv2 + v * uv3;
//        } else {
//            si.uv = Point2f{u + v, v};
//        }
//
//        return true;
//    }
//    return false;
//}
//
//Boolean Triangle::IntersectP(const nrt::Ray &ray, Float tMax) const {
//    const Point3f &v0 = pMesh->positions[mIndices[0]];
//    const Point3f &v1 = pMesh->positions[mIndices[1]];
//    const Point3f &v2 = pMesh->positions[mIndices[2]];
//
//    // en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
//    Vector3f edge1 = v1 - v0;
//    Vector3f edge2 = v2 - v0;
//    Vector3f rayCrossEdge2 = Cross(ray.d, edge2);
//    Float det = Dot(edge1, rayCrossEdge2);
//    if (det < Epsilon) {
//        return false;
//    }
//
//    Float invDet = 1 / det;
//    Vector3f s = ray.o - v0;
//    Float u = Dot(s, rayCrossEdge2) * invDet;
//    if (u < 0 || u > 1) {
//        return false;
//    }
//
//    Vector3f sCrossEdge1 = Cross(s, edge1);
//    Float v = Dot(ray.d, sCrossEdge1) * invDet;
//    if (v < 0 || u + v > 1) {
//        return false;
//    }
//
//    Float t = Dot(edge2, sCrossEdge1) * invDet;
//
//    if (t > Epsilon) {
//        return true;
//    }
//    return false;
//}
//
//Bound3f Triangle::BoundingBox() const {
//    const Point3f &v0 = pMesh->positions[mIndices[0]];
//    const Point3f &v1 = pMesh->positions[mIndices[1]];
//    const Point3f &v2 = pMesh->positions[mIndices[2]];
//    return Union(Bound3f{v0, v1}, v2);
//}
//
//void
//Triangle::Sample(ShapeRecord &sRec, const Point2f &sample) const {
//    auto UniformSampleTriangle = [](const Point2f &u) -> Point2f {
//        Float su0 = Sqrt(u[0]);
//        return {1 - su0, u[1] * su0};
//    };
//    const Point3f &p0 = pMesh->positions[mIndices[0]];
//    const Point3f &p1 = pMesh->positions[mIndices[1]];
//    const Point3f &p2 = pMesh->positions[mIndices[2]];
//    Point2f b = UniformSampleTriangle(sample);
//
//    sRec.p = b[0] * p0 + b[1] * p1 + (1 - b[0] - b[1]) * p2;
//
//    if (!pMesh->normals.empty()) {
//        const Normal3f &n0 = pMesh->normals[mIndices[0]];
//        const Normal3f &n1 = pMesh->normals[mIndices[1]];
//        const Normal3f &n2 = pMesh->normals[mIndices[2]];
//        sRec.n = Normalize(b[0] * n0 + b[1] * n1 + (1 - b[0] - b[1]) * n2);
//    } else {
//        sRec.n = Normalize(Normal3f{Cross(p1 - p0, p2 - p0)});
//    }
//
//    sRec.pdf = 1 / Area();
//}
//
//Float
//Triangle::Area() const {
//    const Point3f &v0 = pMesh->positions[mIndices[0]];
//    const Point3f &v1 = pMesh->positions[mIndices[1]];
//    const Point3f &v2 = pMesh->positions[mIndices[2]];
//    return 0.5 * Length(Cross(v1 - v0, v2 - v0));
//}

NAMESPACE_END