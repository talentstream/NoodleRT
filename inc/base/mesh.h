//
// Created by 44772 on 2024/2/4.
//

#pragma once

#include "core/object.h"
#include <vector>

NAMESPACE_BEGIN

    class Mesh : public Object {
    public:
        explicit Mesh(const PropertyList &);

        virtual ~Mesh() = default;

        void AddChild(Object *object) override;

        void Initialize() override;

        [[nodiscard]] EClassType GetClassType() const override {
            return EClassType::EMesh;
        }

    public:
        std::vector<class Primitive *> primitives;
        std::vector<Point3f> positions;
        std::vector<Normal3f> normals;
    private:
        void LoadMesh();

    private:
        class BxDF *pBxDF{nullptr};

        std::string_view mFileName;
    };

NAMESPACE_END