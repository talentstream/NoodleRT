//
// Created by 44772 on 2024/2/4.
//

#pragma once

#include "core/object.h"
#include <vector>

NAMESPACE_BEGIN

class Mesh : public Object {
public:
    explicit Mesh(const PropertyList &propertyList);

    virtual ~Mesh() = default;

    void AddChild(Object *child) override;

    void Initialize() override;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EMesh;
    }

private:
    void LoadMesh();

public:
    std::vector<class Primitive *> primitives;
    std::vector<Point3f> positions;
    std::vector<Normal3f> normals;
    std::vector<Point2f> uvs;

private:
    class BxDF *pBxDF{nullptr};

    std::string_view mFileName;
    Transform mObjectToWorld;
};

NAMESPACE_END