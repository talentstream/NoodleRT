//
// Created by 44772 on 2024/1/27.
//
#include "base/camera.h"
#include <print>

NAMESPACE_BEGIN

    class PerspectiveCamera : public Camera {
    public:
        //Todo: refactor camera code
        PerspectiveCamera(const PropertyList &propertyList) {
            mWidth = propertyList.GetInteger("width", 400);
            mHeight = propertyList.GetInteger("height", 400);
            mCenter = propertyList.GetPoint("center", {});
            PRINT_DEBUG_INFO("Camera", "perspective");
        }

        void Initialize() override {
            Float aspectRatio = Float(mWidth) / Float(mHeight);
            Float focalLength = 1.0f;
            Float viewportHeight = 2.0f;
            Float viewportWidth = aspectRatio * viewportHeight;

            Vector3f viewPortU = {viewportWidth, 0, 0};
            Vector3f viewPortV = {0, -viewportHeight, 0};

            deltaU = viewPortU / Float(mWidth);
            deltaV = viewPortV / Float(mHeight);

            Vector3f viewPortUpperLeft{mCenter - viewPortU / 2 - viewPortV / 2 - Vector3f(0, 0, focalLength)};
            pixel100Loc = Point3f{viewPortUpperLeft + deltaU * 0.5 + deltaV * 0.5};
        }

        Ray GenerateRay(Point2f uv) const override {
            auto PixelSampleSquare = [this]() {
                auto pu = -0.5f + RandomFloat();
                auto pv = -0.5f + RandomFloat();
                return pu * deltaU + pv * deltaV;
            };

            auto pixelCenter = pixel100Loc + uv.x * deltaU + uv.y * deltaV;
            auto pixelSample = pixelCenter + PixelSampleSquare();
            return Ray{mCenter, pixelSample - mCenter};
        }

    private:
        Integer mWidth;
        Integer mHeight;
        Point3f mCenter;

        Vector3f deltaU;
        Vector3f deltaV;
        Point3f pixel100Loc;
    };

    REGISTER_CLASS(PerspectiveCamera, "perspective")

NAMESPACE_END