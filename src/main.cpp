#include "libs.hpp"
#include "ProtoEngine.hpp"

using namespace Engine;

struct Rotate : public Behavior
{
    f32 angle = 0;
    const f32 speed = 30.f;
    Rotate()
    {
        start = []() {};
        update = [&]()
        {
            angle += Time::deltaTime * speed;
            entity->transform.rotation.y = angle;
        };
    }
};
struct Move : public Behavior
{
    f32 angle = 0.f;
    const f32 RANGE = 13.f;
    const f32 SPEED = 30.f;
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
    const f32 MOVEMENT_SPEED = 10.f;
    const f32 MOUSE_SENSITIVITY = 4.f;
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
        f32 xOff = Input::MouseMovement.x * Time::deltaTime * MOUSE_SENSITIVITY;
        f32 yOff = Input::MouseMovement.y * Time::deltaTime * MOUSE_SENSITIVITY;

        entity->transform.rotation.y += xOff;
        entity->transform.rotation.x -= yOff;
        if (entity->transform.rotation.x > 89.0f)
            entity->transform.rotation.x = 89.0f;
        if (entity->transform.rotation.x < -89.0f)
            entity->transform.rotation.x = -89.0f;
    }
    void processKeyboard(Direction dir)
    {
        f32 vel = MOVEMENT_SPEED * Time::deltaTime;

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
    Engine::init(scene, "Engine", true);

    scene.skybox = new Skybox({"skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg"});

    Entity *camera = new Entity("Main Camera");
    camera->addComponent<Camera>(activeWindow->width, activeWindow->height)->layers.push_back("Back");
    camera->addComponent<PlayerMovement>();

    activeScene->setMainCamera(camera->getComponent<Camera>());

    Entity::loadModel("Landscape.fbx");

    Entity *sun = new Entity("Sun");
    sun->addComponent<DirectionalLight>(glm::vec3(0.4f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 1);
    sun->transform.rotation = {1, -0.35, 0.18};

    Engine::run();

    return 0;
}