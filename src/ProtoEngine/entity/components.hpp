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
        glm::vec3 getWorldPosition() const;
        glm::vec3 getWorldFront() const;
        glm::vec3 getWorldUp() const;
        glm::vec3 getWorldRight() const;
        void update();
        void lateUpdate();
        bool hasMoved() const;

    private:
        glm::vec3 lastPosition;
    };

    ///
    ///
    ///
    class Camera : public Component
    {
    public:
        Camera(bool isMainCamera = false);
        ~Camera();

    public:
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        void renderToTexture();

    public:
        RenderTexture *targetTexture;
        std::vector<std::string> layers = {"Default"};
        float fov;
    };

    ///
    ///
    /// TODO: add rendering priority / rendering order
    class MeshRenderer : public Component
    {
    public:
        MeshRenderer(bool addToScene = true);
        MeshRenderer(const MeshRenderer &other);
        ~MeshRenderer();

    public:
        void setMesh(const Mesh &mesh);
        void setMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
        void addToMesh(const Mesh &mesh);
        void addToMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
        void drawMesh();

    public:
        Material *material;
        Mesh mesh;
        Bounds bounds;
        bool drawBounds = false;
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
        virtual const std::vector<float> getData() const;

    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float intensity;
    };

    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity);
        ~DirectionalLight();

    public:
        glm::vec3 getDirection() const;
        const std::vector<float> getData() const override;

    public:
        static const uint32_t NUM_DATA = 16;
    };

    class PointLight : public Light
    {
    public:
        PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity, float range);
        ~PointLight();

    public:
        const std::vector<float> getData() const override;

    public:
        float range;

        static const uint32_t NUM_DATA = 20;
    };

    class SpotLight : public Light
    {
    public:
        SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float intensity, float range, float innerCutoff, float outerCutoff);
        ~SpotLight();

    public:
        glm::vec3 getDirection() const;
        const std::vector<float> getData() const override;

    public:
        float innerCutoff;
        float outerCutoff;
        float range;

        static const uint32_t NUM_DATA = 28;
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
        std::function<void()> start = []() {};
        std::function<void()> update = []() {};
        std::function<void()> lateUpdate = []() {};
    };
}
#endif // SRC_ENGINE_ENTITY_COMPONENTS
