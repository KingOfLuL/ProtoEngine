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
        : inputListener(std::bind(updateMouseMovement, this))
    {
        start = [&]()
        {
            Input::MouseMoveEvent += &inputListener;
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

    Listener inputListener;

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

const std::vector<Vertex> &verts = {
    Vertex({-1, 1, 0}, {0, 0, 1}, {0, 1}),
    Vertex({-1, -1, 0}, {0, 0, 1}, {0, 0}),
    Vertex({1, -1, 0}, {0, 0, 1}, {1, 0}),
    Vertex({1, 1, 0}, {0, 0, 1}, {1, 1}),
};
const std::vector<u32> indices = {0, 1, 3, 1, 2, 3};

int main()
{
    application = new Application("App", true);

    Scene scene("Game");
    application->setActiveScene(&scene);

    Shader backScreenShader("vertex/back.vs.glsl", "fragment/back.fs.glsl", "Back");
    scene.skybox = new Skybox({"skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg"});

    Entity camera("Main Camera");
    camera.addComponent<PlayerMovement>();
    auto cameraComponent = camera.addComponent<Camera>(application->window->width,
                                                       application->window->height,
                                                       TextureType::COLOR_TEXTURE | TextureType::DEPTH_TEXTURE | TextureType::MULTISAMPLE);
    cameraComponent->layers.push_back("Back");
    application->scene->setMainCamera(cameraComponent);

    // Entity entBackCamera("Back Camera");
    // entBackCamera.setParent(&camera);
    // auto backCamera = entBackCamera.addComponent<Camera>(application->window->width * 0.25,
    //                                                      application->window->height * 0.25,
    //                                                      TextureType::DEPTH_TEXTURE, Camera::ProjectionType::ORTHOGRAPHIC);

    Entity *sun = new Entity("Sun");
    sun->addComponent<DirectionalLight>(glm::vec3(0.05), glm::vec3(0.5), glm::vec3(0.5), 1);
    sun->addComponent<Rotate>();
    sun->transform.rotation.x = -45.f;

    Entity backScreen("Back Screen");
    backScreen.transform.scale = glm::vec3(0.25);
    backScreen.transform.position.y = 0.7;
    backScreen.layer = "Back";
    auto backScreenRenderer = backScreen.addComponent<MeshRenderer>();
    backScreenRenderer->setMesh(verts, indices);
    backScreenRenderer->material = new Material("BackScreen");

    Entity light("Light");
    light.addComponent<PointLight>(glm::vec3(0.0f), glm::vec3(1, 0.5, 0.1), glm::vec3(1, 0.5, 0.1), 1, 10);
    light.setParent(&camera);

    Entity::loadModel("Model.fbx");

    application->run();

    return 0;
}