//
// Created by 44772 on 2024/1/27.
//
#include "base/camera.h"
#include <print>

NAMESPACE_BEGIN

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const PropertyList& propertyList){
        std::print("->PerspectiveCamera\n");
    }

    Ray GenerateRay(nrt::Point2f uv) const override{
        return {};
    }
};

REGISTER_CLASS(PerspectiveCamera, "perspective")

NAMESPACE_END