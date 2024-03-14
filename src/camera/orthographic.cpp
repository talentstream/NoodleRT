//
// Created by talentstream on 2024/3/14.
//

#include "base/camera.h"
#include "base/film.h"

NAMESPACE_BEGIN

class Orthographic : public Camera {
public:
    explicit Orthographic(const PropertyList &propertyList) {
        m_cameraToWorld = propertyList.GetTransform("toworld", {});
        m_nearClip = propertyList.GetFloat("near", 1e-2f);
        m_farClip = propertyList.GetFloat("far", 10000.f);
    }

    void
    Initialize() override {
        Camera::Initialize();
        Float width = pFilm->width;
        Float height = pFilm->height;
        Float aspect = width / height;
        m_invResolution = {1.f / width, 1.f / height};
        m_sampleToCamera = Transform::Scale(-.5f, -.5f * aspect, 1.f) *
                           Transform::Translate(-1.f, -1.f / aspect, 0.f) *
                           Transform::Orthographic(m_nearClip, m_farClip);

        m_sampleToCamera.inverse();
    }

    Ray
    GenerateRay(Point2f uv, Point2f u) const override {
        Point3f samplePosition{(uv.x + u.x) * m_invResolution.x, (uv.y + u.y) * m_invResolution.y, 0};
        Point3f nearP = m_sampleToCamera(samplePosition);

        Point3f ro = m_cameraToWorld(Point3f{nearP.x, nearP.y, 0});
        Vector3f rd = Normalize(m_cameraToWorld(Vector3f{0, 0, -1}));

        return {ro, rd, m_nearClip};
    }

private:
    Transform m_cameraToWorld;
    Transform m_sampleToCamera;
    Float m_nearClip;
    Float m_farClip;
    Vector2f m_invResolution;
};

REGISTER_CLASS(Orthographic, "orthographic")

NAMESPACE_END