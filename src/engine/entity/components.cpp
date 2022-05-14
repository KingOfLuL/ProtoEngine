#include "libs.hpp"

#include "components.hpp"

#include "engine.hpp"
#include "util/util.hpp"
#include "renderer/material/shader/shader.hpp"
#include "util/vertices.hpp"
#include "renderer/texture/texture.hpp"
#include "time/time.hpp"
#include "input/input.hpp"

#include <stb/stb_image.h>

#include <glm/gtx/rotate_vector.hpp>

namespace Engine
{
    Transform::Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), scale(scl), rotation(rot),
          localUp(0.0f, 1.0f, 0.0f), localRight(1.0f, 0.0f, 0.0f), localFront(0.0f, 0.0f, 1.0f) {}

    glm::mat4 Transform::getTransformationMatrix() const
    {
        glm::mat4 matrix(1.0f);
        matrix = glm::translate(matrix, position);
        matrix = glm::scale(matrix, scale);
        matrix = glm::rotate(matrix, glm::radians(rotation.x), GeomUtil::X_AXIS); // TODO: Figure out how rotations work (quaternions)
        matrix = glm::rotate(matrix, glm::radians(rotation.y), GeomUtil::Y_AXIS);
        matrix = glm::rotate(matrix, glm::radians(rotation.z), GeomUtil::Z_AXIS);

        if (entity->parent)
            matrix = entity->parent->transform.getTransformationMatrix() * matrix;

        return matrix;
    }
    void Transform::update()
    {
        localFront = glm::vec3(sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.z)),
                               sin(glm::radians(rotation.z)),
                               cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.z)));
        localRight = glm::normalize(glm::cross(localFront, {0.0f, 1.0f, 0.0f}));
        localUp = glm::normalize(glm::cross(localRight, localFront));
    }

    ///
    ///
    ///
    Camera::Camera() : targetTexture(activeWindow->width, activeWindow->height), fov(75.f) {}
    Camera::~Camera()
    {
        if (activeScene->mainCamera == this)
            activeScene->mainCamera = nullptr;
    }
    glm::mat4 Camera::getViewMatrix() const
    {
        return glm::lookAt(entity->transform.position, entity->transform.position + entity->transform.localFront, entity->transform.localUp);
    }
    glm::mat4 Camera::getProjectionMatrix() const
    {
        return glm::perspective<float>(glm::radians(fov), float(activeWindow->width) / float(activeWindow->height), 0.01f, 100.0f);
    }
    glm::mat4 Camera::getOrthoProjectionMatrix() const
    {
        return glm::ortho<float>(0.f, activeWindow->width, 0.f, activeWindow->height);
    }

    ///
    ///
    ///
    MeshRenderer::MeshRenderer()
    {
        activeScene->addMeshRenderer(this);
    }
    MeshRenderer::~MeshRenderer()
    {
        activeScene->removeMeshRenderer(this);
        m_Mesh.vertexbuffer.deleteBuffers();
    }
    MeshRenderer::MeshRenderer(const MeshRenderer &other) : material(other.material)
    {
        m_Mesh = other.m_Mesh;
        activeScene->addMeshRenderer(this);
    }
    void MeshRenderer::setMesh(const Mesh &mesh)
    {
        m_Mesh = mesh;
        m_Mesh.vertexbuffer = Vertexbuffer(&m_Mesh.vertices[0], m_Mesh.vertices.size());
        m_Mesh.vertexbuffer.addIndexbuffer(&m_Mesh.indices[0], m_Mesh.indices.size());

        bounds = m_Mesh.bounds;
    }
    void MeshRenderer::drawMesh()
    {
        glm::mat4 transformation = entity->transform.getTransformationMatrix();
        bounds.center = transformation * glm::vec4(m_Mesh.bounds.center, 1.0);
        bounds.size = transformation * glm::vec4(m_Mesh.bounds.size, 0.0);

        m_Mesh.vertexbuffer.draw();
    }
    void MeshRenderer::drawBounds()
    {
        glm::mat4 transformation = entity->transform.getTransformationMatrix();
        bounds.center = transformation * glm::vec4(m_Mesh.bounds.center, 1.0);
        bounds.size = transformation * glm::vec4(m_Mesh.bounds.size, 0.0);

        bounds.updateCornerVertices();
        bounds.draw();
    }

    ///
    ///
    ///
    Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity)
        : ambient(ambient), diffuse(diffuse), specular(specular), intensity(intensity)
    {
    }
    Light::~Light()
    {
    }
    inline std::vector<float> Light::getData() const
    {
        return {0.f};
    }
    int Light::internal_getIndex() const
    {
        return m_Index;
    }
    void Light::internal_setIndex(int i)
    {
        m_Index = i;
    }

    DirectionalLight::DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity)
        : Light(ambient, diffuse, specular, intensity)
    {
        activeScene->addDirectionalLight(this);
    }
    DirectionalLight::~DirectionalLight()
    {
        activeScene->removeDirectionalLight(this);
    }
    glm::vec3 DirectionalLight::getDirection() const
    {
        return entity->transform.rotation;
    }
    std::vector<float> DirectionalLight::getData() const
    {
        return {
            getDirection().x,
            getDirection().y,
            getDirection().z,
            0.f, // padding
            ambient.x * intensity,
            ambient.y * intensity,
            ambient.z * intensity,
            0.f, // padding
            diffuse.x * intensity,
            diffuse.y * intensity,
            diffuse.z * intensity,
            0.f, // padding
            specular.x * intensity,
            specular.y * intensity,
            specular.z * intensity,
            0.f, // padding
        };
        ;
    }

    PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity, float range)
        : Light(ambient, diffuse, specular, intensity), range(range)
    {
        activeScene->addPointLight(this);
    }
    PointLight::~PointLight()
    {
        activeScene->removePointLight(this);
    }
    std::vector<float> PointLight::getData() const
    {
        return {
            entity->transform.position.x,
            entity->transform.position.y,
            entity->transform.position.z,
            0.f, // padding
            ambient.x * intensity,
            ambient.y * intensity,
            ambient.z * intensity,
            0.f, // padding
            diffuse.x * intensity,
            diffuse.y * intensity,
            diffuse.z * intensity,
            0.f, // padding
            specular.x * intensity,
            specular.y * intensity,
            specular.z * intensity,
            1.f,
            4.5f / range,
            75.f / (range * range),
            0.f, // padding
            0.f, // padding
        };
    }

    SpotLight::SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity, float range, float innerCutoff, float outerCutoff)
        : Light(ambient, diffuse, specular, intensity), innerCutoff(innerCutoff), outerCutoff(outerCutoff), range(range)
    {
        activeScene->addSpotLight(this);
    }
    SpotLight::~SpotLight()
    {
        activeScene->removeSpotLight(this);
    }
    glm::vec3 SpotLight::getDirection() const
    {
        return entity->transform.localFront;
    }
    std::vector<float> SpotLight::getData() const
    {
        return {
            entity->transform.position.x,
            entity->transform.position.y,
            entity->transform.position.z,
            0.f, // padding
            getDirection().x,
            getDirection().y,
            getDirection().z,
            0.f, // padding
            ambient.r * intensity,
            ambient.g * intensity,
            ambient.b * intensity,
            0.f, // padding
            diffuse.r * intensity,
            diffuse.g * intensity,
            diffuse.b * intensity,
            0.f, // padding
            specular.r * intensity,
            specular.g * intensity,
            specular.b * intensity,
            glm::cos(glm::radians(innerCutoff)),
            glm::cos(glm::radians(outerCutoff)),
            1.f,
            4.5f / range,
            75.f / (range * range),
        };
    }

    ///
    ///
    ///
    Behavior::Behavior()
    {
        activeScene->addBehavior(this);
    }
    Behavior::~Behavior()
    {
        activeScene->removeBehavior(this);
    }
}