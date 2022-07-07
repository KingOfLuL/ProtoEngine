#ifndef SRC_ENGINE_ENTITY_COMPONENTS
#define SRC_ENGINE_ENTITY_COMPONENTS

#include "renderer/material/material.hpp"
#include "renderer/mesh/mesh.hpp"
#include "util/types.h"

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
        Camera(i32 resW, i32 resH);
        ~Camera();

    public:
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        void renderToTexture();

    public:
        RenderTexture *targetTexture;
        std::vector<std::string> layers = {"Default"};
        f32 fov;
        glm::vec2 resolution;
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
        void setMesh(const std::vector<Vertex> &vertices, const std::vector<u32> &indices);
        void addToMesh(const Mesh &mesh);
        void addToMesh(const std::vector<Vertex> &vertices, const std::vector<u32> &indices);
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
        Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity);
        virtual ~Light();

    public:
        virtual const std::vector<f32> getData() const;

    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        f32 intensity;
    };

    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity);
        ~DirectionalLight();

    public:
        glm::vec3 getDirection() const;
        const std::vector<f32> getData() const override;

    public:
        static const u32 NUM_DATA = 16;
    };

    class PointLight : public Light
    {
    public:
        PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity, f32 range);
        ~PointLight();

    public:
        const std::vector<f32> getData() const override;

    public:
        f32 range;

        static const u32 NUM_DATA = 20;
    };

    class SpotLight : public Light
    {
    public:
        SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, f32 intensity, f32 range, f32 innerCutoff, f32 outerCutoff);
        ~SpotLight();

    public:
        glm::vec3 getDirection() const;
        const std::vector<f32> getData() const override;

    public:
        f32 innerCutoff;
        f32 outerCutoff;
        f32 range;

        static const u32 NUM_DATA = 28;
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
