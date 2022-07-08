#include "libs.hpp"

#include "components.hpp"

#include "application/application.hpp"
#include "entity/entity.hpp"
#include "util/util.hpp"
#include "renderer/material/shader/shader.hpp"
#include "renderer/texture/texture.hpp"
#include "time/time.hpp"
#include "input/input.hpp"

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
        glm::quat quat_rotation(glm::radians(rotation));
        glm::mat4 rotationMatrix = glm::toMat4(quat_rotation);

        matrix = glm::translate(matrix, position);
        matrix = glm::scale(matrix, scale);
        matrix *= rotationMatrix;

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
    glm::vec3 Transform::getWorldFront() const
    {
        if (entity->parent)
            return entity->parent->transform.getTransformationMatrix() * glm::vec4(localFront, 0.f);
        return localFront;
    }
    glm::vec3 Transform::getWorldUp() const
    {
        if (entity->parent)
            return entity->parent->transform.getTransformationMatrix() * glm::vec4(localUp, 0.f);
        return localUp;
    }
    glm::vec3 Transform::getWorldRight() const
    {
        if (entity->parent)
            return entity->parent->transform.getTransformationMatrix() * glm::vec4(localRight, 0.f);
        return localRight;
    }
    void Transform::update()
    {
        localFront = glm::vec3(glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)),
                               -glm::sin(glm::radians(rotation.x)),
                               glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)));
        localRight = glm::normalize(glm::cross(localFront, {0.0f, 1.0f, 0.0f}));
        localUp = glm::normalize(glm::cross(localRight, localFront));
    }
    void Transform::lateUpdate()
    {
        lastPosition = getWorldPosition();
    }
    bool Transform::hasMoved() const
    {
        return getWorldPosition() != lastPosition;
    }

    ///
    ///
    ///
    Camera::Camera(i32 resW, i32 resH)
        : fov(75.f), resolution(resW, resH)
    {
        application->scene->addCamera(this);
        targetTexture = new RenderTexture(resolution.x, resolution.y);
    }
    Camera::~Camera()
    {
        if (application->scene->mainCamera == this)
        {
            application->scene->mainCamera = nullptr;
        }
        application->scene->removeCamera(this);
        delete targetTexture;
    }
    glm::mat4 Camera::getViewMatrix() const
    {
        return glm::lookAt(entity->transform.getWorldPosition(),
                           entity->transform.getWorldPosition() + entity->transform.getWorldFront(),
                           entity->transform.getWorldUp());
    }
    glm::mat4 Camera::getProjectionMatrix() const
    {
        return glm::perspective<f32>(glm::radians(fov),
                                     f32(targetTexture->width) / f32(targetTexture->height),
                                     0.01f,
                                     300.0f);
    }
    void Camera::renderToTexture()
    {
        targetTexture->bindFramebuffer();
        glViewport(0, 0, targetTexture->width, targetTexture->height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::shaderUniformbufferInput.setData(glm::value_ptr(entity->transform.getWorldPosition()), sizeof(glm::vec3), 0);

        glm::mat4 matrices[] = {
            getViewMatrix(),
            getProjectionMatrix(),
        };
        Renderer::shaderUniformbufferMatrices.setData(&matrices[0], sizeof(matrices), 0);

        if (application->scene->skybox && std::find(layers.begin(), layers.end(), "Default") != layers.end())
            application->scene->skybox->draw();

        for (const auto &renderer : application->scene->getRenderers(this))
        {
            // check if the renderer is on the same layer as the camera
            if (std::find(layers.begin(), layers.end(), renderer->entity->layer) == layers.end())
                continue;

            Material *material = renderer->material;

            if (material->twoSided)
                glDisable(GL_CULL_FACE);
            else
                glEnable(GL_CULL_FACE);

            material->shader->use();

            material->shader->setMat4("_ModelMatrix", renderer->entity->transform.getTransformationMatrix());

            for (u32 i = 0; i < material->textures.size(); i++)
            {
                auto tex = material->textures[i];

                glActiveTexture(GL_TEXTURE0 + i);

                if (tex->getType() == TextureType::DIFFUSE)
                    material->shader->setInt("_Material.DiffuseTexture", i);
                else if (tex->getType() == TextureType::SPECULAR)
                    material->shader->setInt("_Material.SpecularTexture", i);
                else if (tex->getType() == TextureType::NORMAL)
                    material->shader->setInt("_Material.NormalMap", i);

                tex->bind();
            }

            material->shader->setVec3("_Material.DiffuseColor", material->diffuseColor);
            material->shader->setFloat("_Material.Shininess", material->shininess);

            material->shader->setBool("_Material.HasDiffuse", material->hasDiffuseTexture);
            material->shader->setBool("_Material.HasSpecular", material->hasSpecularTexture);
            material->shader->setBool("_Material.HasNormal", material->hasNormalMap);

            renderer->drawMesh();
        }
        targetTexture->unbindFramebuffer();
    }

    ///
    ///
    ///
    MeshRenderer::MeshRenderer()
    {
        application->scene->addMeshRenderer(this);
    }
    MeshRenderer::~MeshRenderer()
    {
        application->scene->removeMeshRenderer(this);
        mesh.vertexbuffer.deleteBuffers();
    }
    MeshRenderer::MeshRenderer(const MeshRenderer &other) : material(other.material), drawBounds(other.drawBounds)
    {
        setMesh(other.mesh);
        application->scene->addMeshRenderer(this);
    }
    void MeshRenderer::setMesh(const Mesh &other)
    {
        mesh = other;
        mesh.vertexbuffer = Vertexbuffer(&mesh.vertices[0], mesh.vertices.size());
        mesh.vertexbuffer.addIndexbuffer(&mesh.indices[0], mesh.indices.size());

        bounds = mesh.bounds;
    }
    void MeshRenderer::setMesh(const std::vector<Vertex> &vertices, const std::vector<u32> &indices)
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
    void MeshRenderer::addToMesh(const std::vector<Vertex> &vertices, const std::vector<u32> &indices)
    {
        mesh.vertices.insert(mesh.vertices.end(), vertices.begin(), vertices.end());
        mesh.indices.insert(mesh.indices.end(), indices.begin(), indices.end());

        mesh.vertexbuffer = Vertexbuffer(&mesh.vertices[0], mesh.vertices.size());

        mesh.calculateBounds();
    }
    void MeshRenderer::drawMesh()
    {
        glm::mat4 transformation = entity->transform.getTransformationMatrix();

        bounds.center = transformation * glm::vec4(mesh.bounds.center, 1.0f);
        bounds.size = transformation * glm::vec4(mesh.bounds.size, 0.0f);

        mesh.vertexbuffer.draw();

        if (drawBounds)
            bounds.draw();
    }

    ///
    ///
    ///
    Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity)
        : ambient(ambient), diffuse(diffuse), specular(specular), intensity(intensity)
    {
    }
    Light::~Light()
    {
    }
    const std::vector<f32> Light::getData() const
    {
        return {0.f};
    }

    DirectionalLight::DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity)
        : Light(ambient, diffuse, specular, intensity)
    {
        application->scene->addDirectionalLight(this);
    }
    DirectionalLight::~DirectionalLight()
    {
        application->scene->removeDirectionalLight(this);
    }
    glm::vec3 DirectionalLight::getDirection() const
    {
        return entity->transform.rotation;
    }
    const std::vector<f32> DirectionalLight::getData() const
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
    }

    PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity, f32 range)
        : Light(ambient, diffuse, specular, intensity), range(range)
    {
        application->scene->addPointLight(this);
    }
    PointLight::~PointLight()
    {
        application->scene->removePointLight(this);
    }
    const std::vector<f32> PointLight::getData() const
    {
        return {
            entity->transform.getWorldPosition().x,
            entity->transform.getWorldPosition().y,
            entity->transform.getWorldPosition().z,
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

    SpotLight::SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity, f32 range, f32 innerCutoff, f32 outerCutoff)
        : Light(ambient, diffuse, specular, intensity), innerCutoff(innerCutoff), outerCutoff(outerCutoff), range(range)
    {
        application->scene->addSpotLight(this);
    }
    SpotLight::~SpotLight()
    {
        application->scene->removeSpotLight(this);
    }
    glm::vec3 SpotLight::getDirection() const
    {
        return entity->transform.localFront;
    }
    const std::vector<f32> SpotLight::getData() const
    {
        return {
            entity->transform.getWorldPosition().x,
            entity->transform.getWorldPosition().y,
            entity->transform.getWorldPosition().z,
            0.f, // padding
            entity->transform.getWorldFront().x,
            entity->transform.getWorldFront().y,
            entity->transform.getWorldFront().z,
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
        application->scene->addBehavior(this);
    }
    Behavior::~Behavior()
    {
        application->scene->removeBehavior(this);
    }
}