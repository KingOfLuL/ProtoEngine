#ifndef SRC_ENGINE_ENTITY_ENTITY
#define SRC_ENGINE_ENTITY_ENTITY

#include "components.hpp"

namespace Engine
{
    class Entity
    {
    public:
        Entity(const glm::vec3 &pos = glm::vec3(0.0f), const glm::vec3 &rot = glm::vec3(0.0f), const glm::vec3 &scl = glm::vec3(1.0f));
        Entity(const Entity &entity);
        ~Entity();

        void forEachChildren(const std::function<void(Entity *)> &function);
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

        template <typename T>
        inline T *addComponent()
        {
            T *component = new T();
            if (dynamic_cast<Component *>(component))
            {
                component->entity = this;
                m_Components.push_back(component);
            }
            return component;
        }

        template <typename T>
        inline T *addComponent(T *component)
        {
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
        Transform transform; // not listed in components
        Entity *parent = nullptr;
        std::vector<Entity *> children;

    private:
        std::vector<Component *> m_Components;
    };
}
#endif // SRC_ENGINE_ENTITY_ENTITY
