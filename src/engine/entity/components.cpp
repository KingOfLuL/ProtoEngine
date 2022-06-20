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

namespace Engine
{
    Transform::Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), scale(scl), rotation(rot),
          localUp(0.0f, 1.0f, 0.0f), localRight(1.0f, 0.0f, 0.0f), localFront(0.0f, 0.0f, 1.0f)
    {
    }

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
    glm::vec3 Transform::getWorldPosition() const
    {
        if (entity->parent)
            return entity->parent->transform.getTransformationMatrix() * glm::vec4(position, 1.f);
        return position;
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
    Camera::Camera(bool isMainCamera) : targetTexture(activeWindow->width, activeWindow->height), fov(75.f)
    {
        if (isMainCamera)
            activeScene->mainCamera = this;
        else
            activeScene->addCamera(this);
    }
    Camera::~Camera()
    {
        if (activeScene->mainCamera == this)
            activeScene->mainCamera = nullptr;
        else
            activeScene->removeCamera(this);
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
    void Camera::renderToTexture()
    {
        targetTexture.bindFramebuffer();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::shaderUniformbufferInput.setData(glm::value_ptr(entity->transform.position), sizeof(glm::vec3), 0);

        glm::mat4 matrices[] = {
            getViewMatrix(),
            getProjectionMatrix(),
        };
        Renderer::shaderUniformbufferMatrices.setData(&matrices[0], sizeof(matrices), 0);

        glActiveTexture(GL_TEXTURE0);
        if (activeScene->skybox)
            activeScene->skybox->draw();

        const auto &renderers = activeScene->getRenderers();
        for (const auto &renderer : renderers)
        {
            Material *material = renderer->material;

            if (material->twoSided)
                glDisable(GL_CULL_FACE);
            else
                glEnable(GL_CULL_FACE);

            material->shader->use();

            material->shader->setMat4("_ModelMatrix", renderer->entity->transform.getTransformationMatrix());
            material->shader->setBool("_Material.hasTransparency", false);

            for (uint32_t i = 0; i < material->textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);

                if (material->textures[i].getType() == TextureType::DIFFUSE)
                    material->shader->setInt("_Material.diffuseTexture", i);
                else if (material->textures[i].getType() == TextureType::SPECULAR)
                    material->shader->setInt("_Material.specularTexture", i);
                if (material->textures[i].colorFormat == GL_RGBA)
                    material->shader->setBool("_Material.hasTransparency", true);

                material->textures[i].bind();
            }

            material->shader->setVec3("_Material.diffuseColor", material->diffuseColor);
            material->shader->setFloat("_Material.shininess", 32.f);

            material->shader->setBool("_Material.hasDiffuse", material->hasDiffuseTexture);
            material->shader->setBool("_Material.hasSpecular", material->hasSpecularTexture);

            renderer->drawMesh();
        }
        targetTexture.unbindFramebuffer();
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
        mesh.vertexbuffer.deleteBuffers();
    }
    MeshRenderer::MeshRenderer(const MeshRenderer &other) : material(other.material), drawBounds(other.drawBounds)
    {
        setMesh(other.mesh);
        activeScene->addMeshRenderer(this);
    }
    void MeshRenderer::setMesh(const Mesh &other)
    {
        mesh = other;
        mesh.vertexbuffer = Vertexbuffer(&mesh.vertices[0], mesh.vertices.size());
        mesh.vertexbuffer.addIndexbuffer(&mesh.indices[0], mesh.indices.size());

        bounds = mesh.bounds;
    }
    void MeshRenderer::setMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    {
        mesh.vertices = vertices;
        mesh.indices = indices;

        mesh.calculateBounds();

        mesh.vertexbuffer = Vertexbuffer(&mesh.vertices[0], mesh.vertices.size());
        mesh.vertexbuffer.addIndexbuffer(&mesh.indices[0], mesh.indices.size());

        bounds = mesh.bounds;
    }
    void MeshRenderer::addToMesh(const Mesh &other)
    {
        mesh.vertices.insert(mesh.vertices.end(), other.vertices.begin(), other.vertices.end());
        mesh.indices.insert(mesh.indices.end(), other.indices.begin(), other.indices.end());

        mesh.vertexbuffer = Vertexbuffer(&mesh.vertices[0], mesh.vertices.size());

        mesh.calculateBounds();
    }
    void MeshRenderer::addToMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    {
        mesh.vertices.insert(mesh.vertices.end(), vertices.begin(), vertices.end());
        mesh.indices.insert(mesh.indices.end(), indices.begin(), indices.end());

        mesh.vertexbuffer = Vertexbuffer(&mesh.vertices[0], mesh.vertices.size());

        mesh.calculateBounds();
    }
    void MeshRenderer::drawMesh()
    {
        glm::mat4 transformation = entity->transform.getTransformationMatrix();

        bounds.center = (transformation * glm::vec4(mesh.bounds.center, 1.0f));
        bounds.size = transformation * glm::vec4(mesh.bounds.size, 0.0f);

        mesh.vertexbuffer.draw();

        if (drawBounds)
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
    const std::vector<float> Light::getData() const
    {
        return {0.f};
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
    const std::vector<float> DirectionalLight::getData() const
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
            true,
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
    const std::vector<float> PointLight::getData() const
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
            true,
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
    const std::vector<float> SpotLight::getData() const
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
            true,
            0.f, // padding
            0.f, // padding
            0.f, // padding
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