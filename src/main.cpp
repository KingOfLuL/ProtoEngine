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
            entity->transform.rotation.y = angle;
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
        entity->transform.rotation.x -= yOff;
        if (entity->transform.rotation.x > 89.0f)
            entity->transform.rotation.x = 89.0f;
        if (entity->transform.rotation.x < -89.0f)
            entity->transform.rotation.x = -89.0f;
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
    Engine::init(scene, "Engine", true, 1920, 1080);

    scene.skybox = new Skybox({"skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg"});

    Shader backScreenShader("vertex/back.vs.glsl", "fragment/back.fs.glsl", "BackScreen");

    Entity *camera = new Entity("Main Camera");
    camera->addComponent<Camera>(true)->layers.push_back("Back");
    camera->addComponent<PlayerMovement>();

    Entity *backScreen = new Entity("BackScreen");
    backScreen->transform.scale *= 0.2f;
    backScreen->transform.position = glm::vec3(0, 0.7, 0);
    backScreen->layer = "Back";
    MeshRenderer *r = backScreen->addComponent<MeshRenderer>();
    r->setMesh({
                   Vertex({-1, 1, 0.0}, {0, 0, 0}, {0.f, 1.f}, {1.f, 1.f, 1.f}),
                   Vertex({-1, -1, 0.0}, {0, 0, 0}, {0.f, 0.f}, {1.f, 1.f, 1.f}),
                   Vertex({1, -1, 0.0}, {0, 0, 0}, {1.f, 0.f}, {1.f, 1.f, 1.f}),
                   Vertex({1, 1, 0.0}, {0, 0, 0}, {1.f, 1.f}, {1.f, 1.f, 1.f}),
               },
               {0, 1, 3, 1, 2, 3});
    r->material = new Material("BackScreen");

    Entity *backCamera = new Entity("Back Camera"); // not rendering back but front
    backCamera->transform.rotation.y = 180;
    backCamera->setParent(camera);
    Camera *bc = backCamera->addComponent<Camera>();
    bc->targetTexture = new RenderTexture(480, 270);

    r->material->textures.push_back(bc->targetTexture->getTexture());

    Entity *flashlight = new Entity("Flashlight");
    flashlight->addComponent<SpotLight>(glm::vec3(0.f), glm::vec3(1, 0, 1), glm::vec3(1, 0, 1), 3, 20, 30, 45);
    flashlight->setParent(camera);

    Entity *box2 = Entity::loadModel("Box.fbx");
    box2->transform.position = glm::vec3(10, 0, 0);

    Entity *tree = Entity::loadModel("Tree.fbx");
    tree->transform.position = glm::vec3(0, 0, 10);
    tree->addComponent<Rotate>();

    Entity *backpack = Entity::loadModel("Backpack.fbx");
    backpack->transform.position = glm::vec3(0, 0, -10);

    Entity *window = Entity::loadModel("Window.fbx");
    window->transform.position = glm::vec3(5, 10, -10);

    Entity *window2 = Entity::loadModel("Window.fbx");
    window2->transform.position = glm::vec3(-10, 10, -10);

    Entity *window3 = Entity::loadModel("Window.fbx");
    window3->transform.position = glm::vec3(15, -10, 5);

    Entity *window4 = Entity::loadModel("Window.fbx");
    window4->transform.position = glm::vec3(0, 5, 25);

    Entity *sun = new Entity("Sun");
    sun->addComponent<DirectionalLight>(glm::vec3(0.2f), glm::vec3(1.f), glm::vec3(1.f), 1);
    sun->transform.rotation.x = 1;

    Engine::run();

    return 0;
}