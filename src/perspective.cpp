//
// Created by 44772 on 2024/1/27.
//
#include "base/camera.h"
#include <print>
NAMESPACE_BEGIN

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const PropertyList& propertyList){
        std::print("PerspectiveCamera::PerspectiveCamera\n");
    }
};

REGISTER_CLASS(PerspectiveCamera, "perspective")

NAMESPACE_END