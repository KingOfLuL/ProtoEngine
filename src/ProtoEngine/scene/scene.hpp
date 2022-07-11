#ifndef SRC_ENGINE_SCENE_SCENE
#define SRC_ENGINE_SCENE_SCENE

#include "entity/entity.hpp"
#include "entity/components.hpp"
#include "renderer/renderer.hpp"
#include "renderer/skybox/skybox.hpp"

namespace Engine
{
    class Scene
    {
    public:
        Scene(const std::string &name);

    public:
        void addEntity(Entity *entity);
        void removeEntity(Entity *entity);
        void addMeshRenderer(MeshRenderer *renderers);
        void removeMeshRenderer(MeshRenderer *renderers);
        void addDirectionalLight(DirectionalLight *light);
        void removeDirectionalLight(DirectionalLight *light);
        void addSpotLight(SpotLight *light);
        void removeSpotLight(SpotLight *light);
        void addPointLight(PointLight *light);
        void removePointLight(PointLight *light);
        void addBehavior(Behavior *behavior);
        void removeBehavior(Behavior *behavior);
        void addCamera(Camera *camera);
        void removeCamera(Camera *camera);
        void setMainCamera(Camera *camera);
        void start();
        void update();
        void lateUpdate();
        const std::list<MeshRenderer *> &getRenderers(Camera *camera);
        const std::list<Camera *> &getCameras() const;
        const std::array<DirectionalLight *, MAX_NR_DIRLIGHTS> &getDirectionalLights() const;
        const std::array<SpotLight *, MAX_NR_SPOTLIGHTS> &getSpotLights() const;
        const std::array<PointLight *, MAX_NR_POINTLIGHTS> &getPointLights() const;

    public:
        Camera *mainCamera = nullptr;
        Skybox *skybox = nullptr;

    private:
        std::string m_Name;
        std::list<Entity *> m_Entities;
        std::list<MeshRenderer *> m_Renderers;
        std::list<Behavior *> m_Behaviors;
        std::list<Camera *> m_Cameras;
        std::array<DirectionalLight *, MAX_NR_DIRLIGHTS> m_DirectionalLights{nullptr};
        std::array<SpotLight *, MAX_NR_SPOTLIGHTS> m_SpotLights{nullptr};
        std::array<PointLight *, MAX_NR_POINTLIGHTS> m_PointLights{nullptr};
    };
}
#endif // SRC_ENGINE_SCENE_SCENE
