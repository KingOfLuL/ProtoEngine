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
    void Component::initialize()
    {
    }

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
    Camera::Camera(i32 resW, i32 resH, i32 texturesToRender, const ProjectionType &projectionType)
        : fov(75.f), resolution(resW, resH), m_ProjectionType(projectionType)
    {
        application->scene->addCamera(this);

        renderTarget = new RenderTexture(resolution.x, resolution.y, texturesToRender);
    }
    Camera::~Camera()
    {
        if (application->scene->mainCamera == this)
        {
            application->scene->mainCamera = nullptr;
        }
        application->scene->removeCamera(this);
        delete renderTarget;
    }
    glm::mat4 Camera::getViewMatrix() const
    {
        if (m_ProjectionType == ProjectionType::ORTHOGRAPHIC)
            return glm::lookAt<f32>(entity->transform.getWorldPosition() - entity->transform.getWorldFront() * 20.f,
                                    entity->transform.getWorldPosition() + entity->transform.getWorldFront(),
                                    entity->transform.getWorldUp());
        else
            return glm::lookAt<f32>(entity->transform.getWorldPosition(),
                                    entity->transform.getWorldPosition() + entity->transform.getWorldFront(),
                                    entity->transform.getWorldUp());
    }
    glm::mat4 Camera::getProjectionMatrix() const
    {
        if (m_ProjectionType == ProjectionType::PERSPECTIVE)
        {
            return glm::perspective<f32>(glm::radians(fov),
                                         f32(renderTarget->width) / f32(renderTarget->height),
                                         nearClipPlane,
                                         farClipPlane);
        }
        if (m_ProjectionType == ProjectionType::ORTHOGRAPHIC)
        {
            const float halfW = orthoSize * 0.5f;
            const float halfH = orthoSize * 0.5;
            return glm::ortho<f32>(-halfW, halfW,
                                   -halfH, halfH,
                                   nearClipPlane, farClipPlane);
        }
        else
        {
            return glm::mat4(1.f);
        }
    }
    void Camera::renderToTexture()
    {
        renderTarget->bindRender();
        glViewport(0, 0, renderTarget->width, renderTarget->height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 matrices[] = {
            getViewMatrix(),
            getProjectionMatrix(),
        };
        Renderer::shaderUniformbufferMatrices.setData(&matrices[0], sizeof(matrices), 0);

        if (application->scene->skybox && std::find(layers.begin(), layers.end(), "Default") != layers.end())
            if (m_ProjectionType == ProjectionType::PERSPECTIVE)
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

            material->shader->setFloat("_NearClipPlane", nearClipPlane);
            material->shader->setFloat("_FarClipPlane", farClipPlane);
            material->shader->setVec3("_ViewPosition", entity->transform.getWorldPosition());

            material->shader->setMat4("_ModelMatrix", renderer->entity->transform.getTransformationMatrix());

            material->shader->setInt("_DepthTexture", 0);
            material->shader->setInt("_ShadowMap", 1);
            material->shader->setInt("_Skybox", 2);

            glActiveTexture(GL_TEXTURE0);
            if (renderTarget->getTexture(TextureType::DEPTH_TEXTURE))
                renderTarget->getTexture(TextureType::DEPTH_TEXTURE)->bind();
            glActiveTexture(GL_TEXTURE1);
            if (application->scene->shadowCaster)
                application->scene->shadowCaster->getCamera()->renderTarget->getTexture(DEPTH_TEXTURE)->bind();
            glActiveTexture(GL_TEXTURE2);
            if (application->scene->skybox)
                application->scene->skybox->bind();

            for (usize i = 0; i < material->textures.size(); i++)
            {
                Texture2D *tex = material->textures[i];
                material->shader->setInt(tex->name, 3 + i);

                glActiveTexture(GL_TEXTURE3 + i);
                tex->bind();
            }

            material->shader->setVec3("_Material.DiffuseColor", material->diffuseColor);
            material->shader->setFloat("_Material.Shininess", material->shininess);

            material->shader->setBool("_Material.HasDiffuse", material->hasDiffuseTexture);
            material->shader->setBool("_Material.HasSpecular", material->hasSpecularTexture);
            material->shader->setBool("_Material.HasNormal", material->hasNormalMap);

            renderer->drawMesh();
        }
        renderTarget->unbindRender();
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

    DirectionalLight::DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity, bool isShadowCaster)
        : Light(ambient, diffuse, specular, intensity), m_IsShadowCaster(isShadowCaster)
    {
        application->scene->addDirectionalLight(this);
        if (m_IsShadowCaster)
            application->scene->shadowCaster = this;
    }
    void DirectionalLight::initialize()
    {
        m_Camera = entity->addComponent<Camera>(
            application->shadowResolution.x,
            application->shadowResolution.y,
            TextureType::DEPTH_TEXTURE,
            Camera::ProjectionType::ORTHOGRAPHIC);
        application->scene->removeCamera(m_Camera);
    }
    DirectionalLight::~DirectionalLight()
    {
        application->scene->removeDirectionalLight(this);
        if (m_IsShadowCaster)
            application->scene->shadowCaster = nullptr;
    }
    void DirectionalLight::renderShadowMap()
    {
        m_Camera->renderToTexture();
    }
    const Camera *DirectionalLight::getCamera() const
    {
        return m_Camera;
    }
    const std::vector<f32> DirectionalLight::getData() const
    {
        return {
            entity->transform.getWorldFront().x,
            entity->transform.getWorldFront().y,
            entity->transform.getWorldFront().z,
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