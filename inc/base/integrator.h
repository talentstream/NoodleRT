//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN


class Camera;

class Sampler;

class Emitter;

class Integrator : public Object {
public:
    Integrator(const PropertyList &propertyList);

    virtual
    ~Integrator() = default;

    virtual
    void Render() const = 0;

    void
    AddChild(Object *child) override;

    void
    Initialize() override;

    EClassType
    GetClassType() const override {
        return EClassType::EIntegrator;
    }

    const Camera
    *GetCamera() const {
        return pCamera;
    }

protected:
    class Aggregate *pAggregate{nullptr};

    std::vector<Emitter *> mLights;

    Camera *pCamera{nullptr};
    Sampler *pSampler{nullptr};
    Int mSpp{};
};

class ImageTileIntegrator : public Integrator {
public:
    ImageTileIntegrator(const PropertyList &propertyList)
            : Integrator(propertyList) {};

    ~ImageTileIntegrator() = default;

    void Render() const final;

    [[nodiscard]] virtual Color3f Li(const Ray &ray) const = 0;
};

NAMESPACE_END
