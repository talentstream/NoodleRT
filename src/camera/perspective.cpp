//
// Created by 44772 on 2024/1/27.
//
#include "base/camera.h"
#include "base/film.h"

NAMESPACE_BEGIN

class PerspectiveCamera : public Camera {
public:
    //Todo: refactor camera code
    PerspectiveCamera(const PropertyList &propertyList) {
        mFov = propertyList.GetFloat("fov", 90);
        mLookFrom = propertyList.GetPoint("lookFrom", {0, 0, 0});
        mLookAt = propertyList.GetPoint("lookAt", {0, 0, -1});
        mUp = propertyList.GetVector("up", {0, 1, 0});
        PRINT_DEBUG_INFO("Camera", "perspective");
    }

    void Initialize() override {
        Camera::Initialize();
        mWidth = pFilm->width;
        mHeight = pFilm->height;
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

    Ray GenerateRay(Point2f uv, Point2f sample) const override {
        auto PixelSampleSquare = [&]() {
            auto pu = -0.5f + sample.x;
            auto pv = -0.5f + sample.y;
            return pu * deltaU + pv * deltaV;
        };

        auto pixelCenter = pixel100Loc + uv.x * deltaU + uv.y * deltaV;
        auto pixelSample = pixelCenter + PixelSampleSquare();

        return Ray{mLookFrom, Normalize(pixelSample - mLookFrom)};
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

class Perspective : public Camera {
public:
    explicit
    Perspective(const PropertyList &propertyList) {
        mCameraToWorld = propertyList.GetTransform("toworld", {});
        mFov = propertyList.GetFloat("fov", 30.f);
        mNearClip = propertyList.GetFloat("near", 1e-2f);
        mFarClip = propertyList.GetFloat("far", 1000.f);

        PRINT_DEBUG_INFO("Camera", "perspective");
    }

    void
    Initialize() override {
        Camera::Initialize();

        Float width = pFilm->width;
        Float height = pFilm->height;
        Float aspect = width / height;
        mInvResolution = {1.f / width, 1.f / height};

        // https://pbr-book.org/3ed-2018/Camera_Models/Projective_Camera_Models
        mSampleToCamera = Transform::Scale(5.f, -5.f * aspect, 1.f) *
                          Transform::Translate(1.f, -1.f / aspect, 0.f) *
                          Transform::Perspective(mFov, mNearClip, mFarClip);


        mSampleToCamera.inverse();

    }

    Ray
    GenerateRay(Point2f uv, Point2f u) const override {
        Point3f samplePosition{(uv.x + u.x) * mInvResolution.x, (uv.y + u.y) * mInvResolution.y, 0};
        Point3f nearP = mSampleToCamera(samplePosition);

        Vector3f d = Normalize(Vector3f{nearP});
        Float invZ = 1.f / d.z;

        Point3f ro = mCameraToWorld(Point3f{0, 0, 0});
        Vector3f rd = mCameraToWorld(d);

        return {ro, rd, mNearClip * invZ};
    }

private:
    Transform mCameraToWorld;
    Transform mSampleToCamera;
    Float mFov;
    Float mNearClip;
    Float mFarClip;
    Vector2f mInvResolution;
};

REGISTER_CLASS(Perspective, "perspective1")

NAMESPACE_END