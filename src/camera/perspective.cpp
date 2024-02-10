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
            mFov = propertyList.GetFloat("fov", 90);
            mLookFrom = propertyList.GetPoint("lookFrom", {0, 0, 0});
            mLookAt = propertyList.GetPoint("lookAt", {0, 0, -1});
            mUp = propertyList.GetVector("up", {0, 1, 0});
            PRINT_DEBUG_INFO("Camera", "perspective");
        }

        void Initialize() override {
            Float aspectRatio = Float(mWidth) / Float(mHeight);
            Float focalLength = Length(mLookFrom - mLookAt);
            auto theta = DegreeToRadian(mFov);
            Float h = Tan(theta / 2.0);
            Float viewportHeight = 2.0f * h * focalLength;
            Float viewportWidth = aspectRatio * viewportHeight;

            Vector3f w = Normalize(mLookFrom - mLookAt);
            Vector3f u = Normalize(Cross(mUp, w));
            Vector3f v = Cross(w, u);

            auto viewportU = viewportWidth * u;
            auto viewportV = viewportHeight * -v;
            deltaU = viewportU / mWidth;
            deltaV = viewportV / mHeight;

            auto viewportUpperLeft = mLookFrom - (w * focalLength + viewportU / 2 + viewportV / 2);
            pixel100Loc = Point3f{viewportUpperLeft + deltaU * 0.5 + deltaV * 0.5};
        }

        Ray GenerateRay(Point2f uv) const override {
            auto PixelSampleSquare = [this]() {
                auto pu = -0.5f + RandomFloat();
                auto pv = -0.5f + RandomFloat();
                return pu * deltaU + pv * deltaV;
            };

            auto pixelCenter = pixel100Loc + uv.x * deltaU + uv.y * deltaV;
//            auto pixelSample = pixelCenter + PixelSampleSquare();
            return Ray{mLookFrom, pixelCenter - mLookFrom};
        }

    private:
        Integer mWidth;
        Integer mHeight;
        Float mFov;
        Point3f mLookFrom;
        Point3f mLookAt;
        Vector3f mUp;

        Vector3f deltaU;
        Vector3f deltaV;
        Point3f pixel100Loc;
    };

    REGISTER_CLASS(PerspectiveCamera, "perspective")

NAMESPACE_END