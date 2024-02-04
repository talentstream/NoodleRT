//
// Created by 44772 on 2024/2/4.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

    class Mesh : public Object {
    public:
        explicit Mesh(const PropertyList &);

        void AddChild(Object *object) override;

        virtual ~Mesh() = default;

        [[nodiscard]] EClassType GetClassType() const override {
            return EClassType::EMesh;
        }

    private:

    };

NAMESPACE_END