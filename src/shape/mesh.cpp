//
// Created by talentstream on 2024/3/4.
//

#include "base/shape.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "triangle.h"
#include <print>

#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>

NAMESPACE_BEGIN

class Mesh : public shape {
public:
    explicit Mesh(const PropertyList &propertyList) {
        mFilename = propertyList.GetString("filename", {});
        mObjectToWorld *= propertyList.GetTransform("rotate", {});
        mObjectToWorld *= propertyList.GetTransform("scale", {});
        mObjectToWorld *= propertyList.GetTransform("translate", {});
        PRINT_DEBUG_INFO("Mesh", "mesh")
    }

    Boolean
    Intersect(UInt32 idx, const Ray &ray, Float tMax, SurfaceInteraction &si) const override {
        Float u, v, t;
        auto tri = mTriangles[idx];
        Boolean hit = tri.Intersect(ray, tMax, u, v, t);
        if (!hit) return false;
        Normal3f outNormal;
        if (tri.hasNormal) {
            outNormal = Normalize((1 - u - v) * tri.n0 + u * tri.n1 + v * tri.n2);
        } else {
            outNormal = Normalize(Normal3f{Cross(tri.p1 - tri.p0, tri.p2 - tri.p0)});
        }
        si = SurfaceInteraction(t, ray(t), outNormal, -ray.d);
        if (tri.hasUV) {
            si.u = (1 - u - v) * tri.uv0.x + u * tri.uv1.x + v * tri.uv1.x;
            si.v = (1 - u - v) * tri.uv0.y + u * tri.uv1.y + v * tri.uv1.y;
        } else {
            si.u = (1 - u - v) * 0 + u * 1 + v * 1;
            si.v = (1 - u - v) * 0 + u * 0 + v * 1;
        }
        return true;
    }

    Boolean
    IntersectP(UInt32 idx, const Ray &ray, Float tMax = Infinity) const override {
        return mTriangles[idx].IntersectP(ray, tMax);
    }

    void
    Sample(ShapeSampleRecord &sRec, const Point2f &sample) const override {

    }

    Float
    Pdf(const ShapeSampleRecord &sRec) const override {
        return 0.f;
    }

    Float
    Area() const = 0 {

    }

    Bound3f
    GetBoundingBox(UInt32 idx) const override {
        return mTriangles[idx].GetBoundingBox();
    }

    Point3f
    GetCentroid(UInt32 idx) const override {
        return mTriangles[idx].GetCentroid();
    }

    void
    Initialize() override {
        shape::Initialize();
        LoadMesh();
    }

private:
    std::vector<triangle> mTriangles;

    Transform mObjectToWorld;
    std::string_view mFilename;

    void
    LoadMesh() {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, mFilename.data())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto &shape: shapes) {
            for (auto i{0}; i < shape.mesh.indices.size(); i += 3) {
                tinyobj::index_t idx0 = shape.mesh.indices[i];
                tinyobj::index_t idx1 = shape.mesh.indices[i + 1];
                tinyobj::index_t idx2 = shape.mesh.indices[i + 2];

                auto LoadPosition = [&](const auto &idx) -> Point3f {
                    return mObjectToWorld(Point3f{attrib.vertices[3 * idx.vertex_index + 0],
                                                  attrib.vertices[3 * idx.vertex_index + 1],
                                                  attrib.vertices[3 * idx.vertex_index + 2]
                    });
                };

                triangle tri(LoadPosition(idx0), LoadPosition(idx1), LoadPosition(idx2));

                if (idx0.normal_index > 0) {
                    auto LoadNormal = [&](const auto &idx) -> Normal3f {
                        return {attrib.normals[3 * idx.normal_index + 0],
                                attrib.normals[3 * idx.normal_index + 1],
                                attrib.normals[3 * idx.normal_index + 2]};
                    };
                    tri.SetNormal(LoadNormal(idx0), LoadNormal(idx1), LoadNormal(idx2));
                }

                if (idx0.texcoord_index > 0) {
                    auto LoadUV = [&](const auto &idx) -> Point2f {
                        return {attrib.texcoords[2 * idx.texcoord_index + 0],
                                1 - attrib.texcoords[2 * idx.texcoord_index + 1]};
                    };
                    tri.SetUV(LoadUV(idx0), LoadUV(idx1));
                }

                mTriangles.emplace_back(tri);
            }
        }
    }
};

NAMESPACE_END