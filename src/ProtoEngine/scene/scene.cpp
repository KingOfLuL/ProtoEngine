#include "libs.hpp"

#include "scene.hpp"

#include "application/application.hpp"
#include "util/util.hpp"

namespace Engine
{
    Scene::Scene(const std::string &name)
        : m_Name(name)
    {
    }
    void Scene::addEntity(Entity *entity)
    {
        m_Entities.push_back(entity);
    }
    void Scene::removeEntity(Entity *entity)
    {
        auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
        if (it != m_Entities.end())
            m_Entities.erase(it);
    }
    void Scene::addMeshRenderer(MeshRenderer *renderer)
    {
        m_Renderers.push_back(renderer);
    }
    void Scene::removeMeshRenderer(MeshRenderer *renderer)
    {
        auto it = std::find(m_Renderers.begin(), m_Renderers.end(), renderer);
        if (it != m_Renderers.end())
            m_Renderers.erase(it);
    }
    void Scene::addDirectionalLight(DirectionalLight *light)
    {
        bool canCreate = false;
        for (i32 i = 0; i < MAX_NR_DIRLIGHTS; ++i)
        {
            if (m_DirectionalLights[i] == nullptr)
            {
                m_DirectionalLights[i] = light;
                canCreate = true;
                break;
            }
        }
        if (!canCreate)
            std::cout << "Cant create a new directional light" << std::endl;
    }
    void Scene::removeDirectionalLight(DirectionalLight *light)
    {
        i32 index = 0;
        for (i32 i = 0; i < MAX_NR_DIRLIGHTS; i++)
        {
            if (m_DirectionalLights[i] == light)
            {
                index = i;
                break;
            }
        }
        m_DirectionalLights[index] = nullptr;
    }
    void Scene::addSpotLight(SpotLight *light)
    {
        bool canCreate = false;
        for (i32 i = 0; i < MAX_NR_SPOTLIGHTS; ++i)
        {
            if (m_SpotLights[i] == nullptr)
            {
                m_SpotLights[i] = light;
                canCreate = true;
                break;
            }
        }
        if (!canCreate)
            std::cout << "Cant create a new spot light" << std::endl;
    }
    void Scene::removeSpotLight(SpotLight *light)
    {
        i32 index = 0;
        for (i32 i = 0; i < MAX_NR_SPOTLIGHTS; i++)
        {
            if (m_SpotLights[i] == light)
            {
                index = i;
                break;
            }
        }
        m_SpotLights[index] = nullptr;
    }
    void Scene::addPointLight(PointLight *light)
    {
        bool canCreate = false;
        for (i32 i = 0; i < MAX_NR_POINTLIGHTS; ++i)
        {
            if (!m_PointLights[i])
            {
                m_PointLights[i] = light;
                canCreate = true;
                break;
            }
        }
        if (!canCreate)
            std::cout << "Cant create a new point light" << std::endl;
    }
    void Scene::removePointLight(PointLight *light)
    {
        i32 index = 0;
        for (i32 i = 0; i < MAX_NR_POINTLIGHTS; i++)
        {
            if (m_PointLights[i] == light)
            {
                index = i;
                break;
            }
        }
        m_PointLights[index] = nullptr;
    }
    void Scene::addBehavior(Behavior *behavior)
    {
        m_Behaviors.push_back(behavior);
    }
    void Scene::removeBehavior(Behavior *behavior)
    {
        auto it = std::find(m_Behaviors.begin(), m_Behaviors.end(), behavior);
        if (it != m_Behaviors.end())
            m_Behaviors.erase(it);
    }
    void Scene::addCamera(Camera *camera)
    {
        m_Cameras.push_back(camera);
    }
    void Scene::removeCamera(Camera *camera)
    {
        auto it = std::find(m_Cameras.begin(), m_Cameras.end(), camera);
        if (it != m_Cameras.end())
            m_Cameras.erase(it);
    }
    void Scene::setMainCamera(Camera *camera)
    {
        mainCamera = camera;
    }
    const std::array<DirectionalLight *, MAX_NR_DIRLIGHTS> &Scene::getDirectionalLights() const
    {
        return m_DirectionalLights;
    }
    const std::array<SpotLight *, MAX_NR_SPOTLIGHTS> &Scene::getSpotLights() const
    {
        return m_SpotLights;
    }
    const std::array<PointLight *, MAX_NR_POINTLIGHTS> &Scene::getPointLights() const
    {
        return m_PointLights;
    }
    const std::vector<MeshRenderer *> &Scene::getRenderers(Camera *camera)
    {
        if (camera->entity->transform.hasMoved())
        {
            std::sort(m_Renderers.begin(), m_Renderers.end(),
                      [](MeshRenderer *a, MeshRenderer *b)
                      {
                          f32 distA = glm::length(application->scene->mainCamera->entity->transform.position - a->bounds.center);
                          f32 distB = glm::length(application->scene->mainCamera->entity->transform.position - b->bounds.center);
                          return distA > distB;
                      });
        }
        return m_Renderers;
    }
    const std::vector<Camera *> &Scene::getCameras() const
    {
        return m_Cameras;
    }
    void Scene::update()
    {
        for (auto e : m_Entities)
            e->transform.update();

        for (auto i : m_Behaviors)
            i->update();
    }
    void Scene::start()
    {
        for (auto i : m_Behaviors)
            i->start();
    }
    void Scene::lateUpdate()
    {
        for (auto i : m_Behaviors)
            i->lateUpdate();
        for (auto i : m_Entities)
            i->transform.lateUpdate();
    }
}