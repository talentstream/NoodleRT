//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

    class Scene : public Object {
    public:
        NON_COPY(Scene)

        explicit Scene(const PropertyList&);

        ~Scene();

        [[nodiscard]] EClassType GetClassType() const override {
            return EClassType::EScene;
        }

    private:
        class Camera *pCamera{nullptr};

        class Integrator *pIntegrator{nullptr};

        class Aggregate *pAggregate{nullptr};

    public:
        const Integrator *GetIntegrator() const {
            return pIntegrator;
        }

        const Camera *GetCamera() const {
            return pCamera;
        }

    };

NAMESPACE_END