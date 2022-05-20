#include "libs.hpp"

#include "ProtoEngine.hpp"

using namespace Engine;

struct Rotate : public Behavior
{
    float angle = 0;
    const float speed = 30.f;
    Rotate()
    {
        start = []() {};
        update = [&]()
        {
            angle += Time::deltaTime * speed;
            entity->transform.rotation = glm::vec3(sin(glm::radians(angle)), 0, cos(glm::radians(angle)));
        };
    }
};
struct Move : public Behavior
{
    float angle = 0.f;
    const float RANGE = 13.f;
    const float SPEED = 30.f;
    Move()
    {
        start = []() {};
        update = [&]()
        {
            angle += Time::deltaTime * SPEED;
            entity->transform.position = glm::vec3(glm::sin(glm::radians(angle)) * RANGE,
                                                   glm::sin(glm::radians(angle)) * glm::cos(glm::radians(angle)) * RANGE,
                                                   glm::cos(glm::radians(angle)) * RANGE);
        };
    }
};
struct PlayerMovement : public Behavior
{
    PlayerMovement()
    {
        start = [&]()
        {
            Input::MouseMoveEvent += std::bind(&PlayerMovement::updateMouseMovement, this);
        };
        update = [&]()
        {
            if (Input::keyIsDown(Keys::W))
                processKeyboard(Direction::FORWARD);
            if (Input::keyIsDown(Keys::S))
                processKeyboard(Direction::BACKWARD);
            if (Input::keyIsDown(Keys::A))
                processKeyboard(Direction::LEFT);
            if (Input::keyIsDown(Keys::D))
                processKeyboard(Direction::RIGHT);
            if (Input::keyIsDown(Keys::Space))
                processKeyboard(Direction::UP);
            if (Input::keyIsDown(Keys::LeftCtrl))
                processKeyboard(Direction::DOWN);
        };
    }

private:
    const float MOVEMENT_SPEED = 10.f;
    const float MOUSE_SENSITIVITY = 4.f;
    enum class Direction
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

private:
    void updateMouseMovement()
    {
        float xOff = Input::MouseMovement.x * Time::deltaTime * MOUSE_SENSITIVITY;
        float yOff = Input::MouseMovement.y * Time::deltaTime * MOUSE_SENSITIVITY;

        entity->transform.rotation.y += xOff;
        entity->transform.rotation.z += yOff;
        if (entity->transform.rotation.z > 89.0f)
            entity->transform.rotation.z = 89.0f;
        if (entity->transform.rotation.z < -89.0f)
            entity->transform.rotation.z = -89.0f;
    }
    void processKeyboard(Direction dir)
    {
        float vel = MOVEMENT_SPEED * Time::deltaTime;

        if (dir == Direction::FORWARD)
            entity->transform.position += entity->transform.localFront * vel;
        if (dir == Direction::BACKWARD)
            entity->transform.position -= entity->transform.localFront * vel;
        if (dir == Direction::LEFT)
            entity->transform.position -= entity->transform.localRight * vel;
        if (dir == Direction::RIGHT)
            entity->transform.position += entity->transform.localRight * vel;
        if (dir == Direction::UP)
            entity->transform.position += glm::vec3(0.f, 1.f, 0.f) * vel;
        if (dir == Direction::DOWN)
            entity->transform.position -= glm::vec3(0.f, 1.f, 0.f) * vel;
    }
};

int main()
{
    Scene scene("Game");
    Engine::init(scene, "Engine", 1920, 1080);

    scene.skybox = new Skybox({"skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg"});

    Entity *camera = (new Entity)->addComponent<Camera>()->entity;
    camera->addComponent<PlayerMovement>();
    scene.mainCamera = camera->getComponent<Camera>();

    Entity *box = loadModel("Box.fbx", &Renderer::shaderLit);
    box->getComponent<MeshRenderer>()->drawBounds = true;

    Entity *tree = loadModel("Tree.fbx", &Renderer::shaderLit);
    tree->forEachChildren(
        [&](Entity *ent)
        {
            auto r = ent->getComponent<MeshRenderer>();
            if (r)
                r->drawBounds = true;
        });
    tree->transform.position = glm::vec3(10, 10, 0);

    Entity *backpack = loadModel("Backpack.fbx", &Renderer::shaderLit);
    backpack->forEachChildren(
        [&](Entity *ent)
        {
            auto r = ent->getComponent<MeshRenderer>();
            if (r)
                r->drawBounds = true;
        });
    backpack->transform.position = glm::vec3(0, -10, 10);

    Entity *sun = new Entity;
    sun->addComponent(new DirectionalLight(glm::vec3(0.2f), glm::vec3(1.f), glm::vec3(1.f), 1));
    sun->addComponent<Rotate>();

    Engine::run();

    return 0;
}