//
// Created by 44772 on 2024/1/27.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

class Film;

class Camera : public Object {
public:
    virtual ~Camera() = default;

    [[nodiscard]] virtual Ray GenerateRay(Point2f uv) const = 0;

    void AddChild(Object *child) override;

    void Initialize() override;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::ECamera;
    }

    const Film *GetFilm() const {
        return pFilm;
    }

protected:
    Film *pFilm{nullptr};
};

NAMESPACE_END