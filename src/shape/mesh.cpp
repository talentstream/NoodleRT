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

class Mesh : public Shape {
public:
    explicit Mesh(const PropertyList &propertyList) {
        mFilename = propertyList.GetString("filename", {});
        mObjectToWorld *= propertyList.GetTransform("rotate", {});
        mObjectToWorld *= propertyList.GetTransform("scale", {});
        mObjectToWorld *= propertyList.GetTransform("translate", {});
        PRINT_DEBUG_INFO("Mesh", "mesh")
    }

    Boolean
    Intersect(UInt32 idx, const Ray &ray, IntersectionRecord &iRec) const override {
        Float u, v, t;
        auto tri = mTriangles[idx];
        if (!tri.Intersect(ray, iRec.t, u, v, t)) {
            return false;
        }

        Normal3f outNormal;
        if (tri.hasNormal) {
            outNormal = Normalize((1 - u - v) * tri.n0 + u * tri.n1 + v * tri.n2);
        } else {
            outNormal = Normalize(Normal3f{Cross(tri.p1 - tri.p0, tri.p2 - tri.p0)});
        }

        iRec = IntersectionRecord(t, ray(t), outNormal);
        iRec.SetFlipNormal(ray.d);
        if (tri.hasUV) {
            iRec.uv = (1 - u - v) * tri.uv0 + u * tri.uv1 + v * tri.uv2;
        } else {
            iRec.uv = Point2f{u + v, v};
        }
        iRec.bxdf = pBxDF;
        iRec.emitter = pEmitter;
        return true;
    }

    Boolean
    IntersectP(UInt32 idx, const Ray &ray, Float tMax) const override {
        return mTriangles[idx].IntersectP(ray, tMax);
    }

    void
    Sample(ShapeRecord &sRec, const Point2f &sample) const override {
        auto idx = static_cast<UInt32>(sample.x * mTriangles.size());
        mTriangles[idx].Sample(sRec, sample);
    }

    Float
    Pdf(const ShapeRecord &sRec) const override {
        return 0.f;
    }

    Float
    Area() const override {
        return 0.f;
    }

    UInt32
    GetPrimitiveCount() const override {
        return mTriangles.size();
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
        Shape::Initialize();
        LoadMesh();
    }

private:
    std::vector<Triangle> mTriangles;

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

                Triangle tri(LoadPosition(idx0), LoadPosition(idx1), LoadPosition(idx2));

                if (idx0.normal_index > 0) {
                    auto LoadNormal = [&](const auto &idx) -> Normal3f {
                        return Normalize(Normal3f{mObjectToWorld(Point3f{attrib.normals[3 * idx.normal_index + 0],
                                                                         attrib.normals[3 * idx.normal_index + 1],
                                                                         attrib.normals[3 * idx.normal_index + 2]})});
                    };
                    tri.SetNormal(LoadNormal(idx0), LoadNormal(idx1), LoadNormal(idx2));
                }

                if (idx0.texcoord_index > 0) {
                    auto LoadUV = [&](const auto &idx) -> Point2f {
                        return {attrib.texcoords[2 * idx.texcoord_index + 0],
                                1 - attrib.texcoords[2 * idx.texcoord_index + 1]};
                    };
                    tri.SetUV(LoadUV(idx0), LoadUV(idx1), LoadUV(idx2));
                }

                mTriangles.emplace_back(tri);
            }
        }

    }
};

REGISTER_CLASS(Mesh, "mesh")

NAMESPACE_END