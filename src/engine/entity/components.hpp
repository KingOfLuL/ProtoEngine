#ifndef SRC_ENGINE_ENTITY_COMPONENTS
#define SRC_ENGINE_ENTITY_COMPONENTS

#include "renderer/material/material.hpp"
#include "renderer/mesh/mesh.hpp"

namespace Engine
{
    class Entity;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

    public:
        Entity *entity;
    };

    ///
    ///
    ///
    class Transform : public Component
    {
    public:
        Transform(const glm::vec3 &pos = glm::vec3(0.0f), const glm::vec3 &rot = glm::vec3(0.0f), const glm::vec3 &scl = glm::vec3(1.0f));

    public:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;

        glm::vec3 localUp;
        glm::vec3 localRight;
        glm::vec3 localFront;

    public:
        glm::mat4 getTransformationMatrix() const;
        void update();
    };

    ///
    ///
    ///
    class Camera : public Component
    {
    public:
        Camera();
        ~Camera();

    public:
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
        glm::mat4 getOrthoProjectionMatrix() const;

    public:
        RenderTexture targetTexture;
        float fov;
    };

    ///
    ///
    ///
    class MeshRenderer : public Component
    {
    public:
        MeshRenderer();
        MeshRenderer(const MeshRenderer &other);
        ~MeshRenderer();

    public:
        void setMesh(const Mesh &mesh);
        void drawMesh();

    public:
        Material *material;
        Bounds bounds;
        bool drawBounds = false;

    private:
        Mesh m_Mesh;
    };

    ///
    ///
    ///
    class Light : public Component
    {
    public:
        Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity);
        virtual ~Light();

    public:
        virtual std::vector<float> getData() const;
        int engine_getIndex() const;
        void engine_setIndex(int i);

    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float intensity;

    private:
        int m_Index;
    };

    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity);
        ~DirectionalLight();

    public:
        glm::vec3 getDirection() const;
        std::vector<float> getData() const override;
    };

    class PointLight : public Light
    {
    public:
        PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity, float range);
        ~PointLight();

    public:
        std::vector<float> getData() const override;

    public:
        float range;
    };

    class SpotLight : public Light
    {
    public:
        SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity, float range, float innerCutoff, float outerCutoff);
        ~SpotLight();

    public:
        glm::vec3 getDirection() const;
        std::vector<float> getData() const override;

    public:
        float innerCutoff;
        float outerCutoff;
        float range;
    };

    ///
    ///
    ///
    class Behavior : public Component
    {
    public:
        Behavior();
        virtual ~Behavior();

    public:
        std::function<void()> start;
        std::function<void()> update;
    };
}
#endif // SRC_ENGINE_ENTITY_COMPONENTS
