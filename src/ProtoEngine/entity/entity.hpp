#ifndef SRC_ENGINE_ENTITY_ENTITY
#define SRC_ENGINE_ENTITY_ENTITY

#include "components.hpp"

namespace Engine
{
    class Entity
    {
    private:
        // used for saving already loaded models, entities get created from this data
        struct Model
        {
            Model(Entity *entity, const std::string &filePath);

            std::string fileName;
            std::string entityName;

            bool hasRenderer;
            Mesh mesh;
            Material *material;

            Model *parent;
            std::vector<Model *> children;
        };

    public:
        Entity(const std::string &name, const glm::vec3 &pos = glm::vec3(0.0f), const glm::vec3 &rot = glm::vec3(0.0f), const glm::vec3 &scl = glm::vec3(1.0f));
        Entity(const Entity &entity);
        Entity(Model *model);
        ~Entity();

    public:
        void forEachChildren(const std::function<void(Entity *)> &function, bool b = true);
        void setParent(Entity *parent);
        Entity *getChildByName(const std::string &name) const;

    public:
        template <typename T>
        inline T *getComponent()
        {
            for (auto component : m_Components)
            {
                if (dynamic_cast<T *>(component))
                    return dynamic_cast<T *>(component);
            }
            return nullptr;
        }

        template <typename T, typename... Ts>
        inline T *addComponent(Ts... args)
        {
            T *component = new T(args...);
            if (dynamic_cast<Component *>(component))
            {
                component->entity = this;
                m_Components.push_back(component);
            }
            return component;
        }

        static Entity *loadModel(const std::string &path);

    public:
        std::string name;
        std::string layer = "Default";
        Transform transform; // not listed in components
        Entity *parent = nullptr;
        std::vector<Entity *> children;

        static std::map<std::string, Model *> s_LoadedModels;

    private:
        std::vector<Component *> m_Components;
    };
}
#endif // SRC_ENGINE_ENTITY_ENTITY
