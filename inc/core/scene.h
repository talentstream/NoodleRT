//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

class Scene : public Object {
public:
    NON_COPY(Scene)

    explicit Scene(const PropertyList &);

    ~Scene();

    void AddChild(Object *object) override;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EScene;
    }

private:
    class Integrator *pIntegrator{nullptr};

    Integer mSpp;
    Integer mWidth;
    Integer mHeight;

public:
    const Integrator *GetIntegrator() const {
        return pIntegrator;
    }

    const Integer GetSpp() const {
        return mSpp;
    }

    const Integer GetWidth() const {
        return mWidth;
    }

    const Integer GetHeight() const {
        return mHeight;
    }
};

NAMESPACE_END