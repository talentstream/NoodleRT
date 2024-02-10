//
// Created by 44772 on 2024/2/11.
//

#include "base/camera.h"
#include "base/film.h"

NAMESPACE_BEGIN

void Camera::AddChild(Object *child) {
    switch (child->GetClassType()) {
        case EClassType::EFilm:
            pFilm = dynamic_cast<Film *>(child);
            break;
        default:
            throw std::runtime_error("Camera Add Error Child:" +
                                     Object::ClassTypeName(child->GetClassType()));
    }
}

void Camera::Initialize() {
    if (pFilm == nullptr) {
        throw std::runtime_error("Camera Need Film!");
    }
}

NAMESPACE_END