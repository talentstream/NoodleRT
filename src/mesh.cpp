//
// Created by 44772 on 2024/2/4.
//

#include "base/mesh.h"
#include "base/bxdf.h"
#include "base/primitive.h"
#include "shape/triangle.cpp"
#include <print>

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

NAMESPACE_BEGIN

    Mesh::Mesh(const PropertyList &propertyList) {
        mFileName = propertyList.GetString("filename", {});
        PRINT_DEBUG_INFO("Mesh", "mesh")
    }

    void Mesh::AddChild(Object *object) {
        switch (object->GetClassType()) {
            case EClassType::EBxDF:
                pBxDF = static_cast<BxDF *>(object);
                break;
        }
    }

    void Mesh::Initialize() {
        if (pBxDF == nullptr) {
            pBxDF = static_cast<BxDF *>(ObjectFactory::CreateInstance("diffuse", PropertyList()));
        }
        LoadMesh();
    }

    void Mesh::LoadMesh() {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, mFileName.data())) {
            throw std::runtime_error(warn + err);
        }

        for (const auto &shape: shapes) {
            for (auto i{0}; i < shape.mesh.indices.size(); i += 3) {
                tinyobj::index_t idx0 = shape.mesh.indices[i];
                tinyobj::index_t idx1 = shape.mesh.indices[i + 1];
                tinyobj::index_t idx2 = shape.mesh.indices[i + 2];
                auto LoadPosition = [&](const auto &idx) -> void {
                    positions.emplace_back(attrib.vertices[3 * idx.vertex_index + 0],
                                           attrib.vertices[3 * idx.vertex_index + 1],
                                           attrib.vertices[3 * idx.vertex_index + 2]);
                };
                LoadPosition(idx0);
                LoadPosition(idx1);
                LoadPosition(idx2);
                auto LoadNormal = [&](const auto &idx) -> void {
                    normals.emplace_back(attrib.normals[3 * idx.normal_index + 0],
                                         attrib.normals[3 * idx.normal_index + 1],
                                         attrib.normals[3 * idx.normal_index + 2]);
                };
                LoadNormal(idx0);
                LoadNormal(idx1);
                LoadNormal(idx2);

                auto *pTriangle = new Triangle(this, i);
                auto primitive = dynamic_cast<Primitive *>(ObjectFactory::CreateInstance("geometry", PropertyList()));
                primitive->AddChild(pTriangle);
                primitives.emplace_back(primitive);
            };
        }

    }

NAMESPACE_END