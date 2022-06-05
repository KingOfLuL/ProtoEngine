#include "libs.hpp"

#include "scene.hpp"

#include "engine.hpp"
#include "util/util.hpp"

namespace Engine
{
    Scene::Scene(const std::string &name) : m_Name(name)
    {
        m_Arr = {new int(1), new int(5), new int(3), new int(2), new int(4)};
    }
    void Scene::addEntity(Entity *entity)
    {
        m_Entities.push_back(entity);
    }
    void Scene::removeEntity(Entity *entity)
    {
        m_Entities.erase(std::find(m_Entities.begin(), m_Entities.end(), entity));
    }
    void Scene::addMeshRenderer(MeshRenderer *renderer)
    {
        m_Renderers.push_back(renderer);
    }
    void Scene::removeMeshRenderer(MeshRenderer *renderer)
    {
        m_Renderers.erase(std::find(m_Renderers.begin(), m_Renderers.end(), renderer));
    }
    void Scene::addDirectionalLight(DirectionalLight *light)
    {
        bool canCreate = false;
        for (int i = 0; i < MAX_NR_DIRLIGHTS; ++i)
        {
            if (m_DirectionalLights[i] == nullptr)
            {
                m_DirectionalLights[i] = light;
                light->internal_engine_setIndex(i);
                canCreate = true;
                break;
            }
        }
        if (!canCreate)
            std::cout << "Cant create a new directional light" << std::endl;
    }
    void Scene::removeDirectionalLight(DirectionalLight *light)
    {
        m_DirectionalLights[light->internal_engine_getIndex()] = nullptr;
    }
    void Scene::addSpotLight(SpotLight *light)
    {
        bool canCreate = false;
        for (int i = 0; i < MAX_NR_SPOTLIGHTS; ++i)
        {
            if (m_SpotLights[i] == nullptr)
            {
                m_SpotLights[i] = light;
                light->internal_engine_setIndex(i);
                canCreate = true;
                break;
            }
        }
        if (!canCreate)
            std::cout << "Cant create a new spot light" << std::endl;
    }
    void Scene::removeSpotLight(SpotLight *light)
    {
        m_SpotLights[light->internal_engine_getIndex()] = nullptr;
    }
    void Scene::addPointLight(PointLight *light)
    {
        bool canCreate = false;
        for (int i = 0; i < MAX_NR_POINTLIGHTS; ++i)
        {
            if (!m_PointLights[i])
            {
                m_PointLights[i] = light;
                light->internal_engine_setIndex(i);
                canCreate = true;
                break;
            }
        }
        if (!canCreate)
            std::cout << "Cant create a new point light" << std::endl;
    }
    void Scene::removePointLight(PointLight *light)
    {
        m_PointLights[light->internal_engine_getIndex()] = nullptr;
    }
    void Scene::addBehavior(Behavior *behavior)
    {
        m_Behaviors.push_back(behavior);
    }
    void Scene::removeBehavior(Behavior *behavior)
    {
        m_Behaviors.erase(std::find(m_Behaviors.begin(), m_Behaviors.end(), behavior));
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
    const std::vector<MeshRenderer *> &Scene::getRenderers()
    {
        std::sort(m_Renderers.begin(), m_Renderers.end(),
                  [](MeshRenderer *a, MeshRenderer *b)
                  {
                      float distA = glm::length(activeScene->mainCamera->entity->transform.position - a->bounds.center);
                      float distB = glm::length(activeScene->mainCamera->entity->transform.position - b->bounds.center);
                      return distA > distB;
                  });
        return m_Renderers;
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
}