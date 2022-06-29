#include "libs.hpp"

#include "renderer.hpp"

#include "engine.hpp"
#include "glad/glad.h"
#include "util/util.hpp"
#include "time/time.hpp"

#include <stb/stb_image.h>

namespace Engine::Renderer
{
    Shader *shaderLit;
    Shader *shaderBounds;
    Shader *shaderCubemap;
    Shader *shaderScreen;

    Uniformbuffer shaderUniformbufferMatrices;
    Uniformbuffer shaderUniformbufferLights;
    Uniformbuffer shaderUniformbufferInput;

    const uint32_t MATRIX_DATA_SIZE = 2 * sizeof(glm::mat4);

    const uint32_t NUN_DIRLIGHTS_DATA = DirectionalLight::NUM_DATA * MAX_NR_DIRLIGHTS;
    const uint32_t NUM_POINTLIGHTS_DATA = PointLight::NUM_DATA * MAX_NR_POINTLIGHTS;
    const uint32_t NUM_SPOTLIGHTS_DATA = SpotLight::NUM_DATA * MAX_NR_SPOTLIGHTS;

    const uint32_t LIGHT_DATA_SIZE = (NUN_DIRLIGHTS_DATA + NUM_POINTLIGHTS_DATA + NUM_SPOTLIGHTS_DATA) * sizeof(float);

    const uint32_t INPUT_DATA_SIZE = 7 * sizeof(float);

    // TODO: add Batched Renderer

    void init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_MULTISAMPLE);

        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);

        shaderLit = new Shader("vertex/vertex.vs.glsl", "fragment/lit.fs.glsl", "Lit");
        shaderCubemap = new Shader("vertex/cubemap.vs.glsl", "fragment/cubemap.fs.glsl", "Cubemap");
        shaderBounds = new Shader("vertex/vertex.vs.glsl", "fragment/bounds.fs.glsl", "Bounds");
        shaderBounds->addGeometryShader("geometry/bounds.gs.glsl");

        shaderUniformbufferMatrices = Uniformbuffer(MATRIX_DATA_SIZE, 0);
        shaderUniformbufferLights = Uniformbuffer(LIGHT_DATA_SIZE, 1);
        shaderUniformbufferInput = Uniformbuffer(INPUT_DATA_SIZE, 2);
    }
    void updateLights()
    {
        auto dirLights = activeScene->getDirectionalLights();
        auto pointLights = activeScene->getPointLights();
        auto spotLights = activeScene->getSpotLights();

        std::array<float, LIGHT_DATA_SIZE / sizeof(float)> lightData;

        int offset = 0;

        for (int i = 0; i < MAX_NR_DIRLIGHTS; i++)
        {
            const uint32_t localOffset = offset + i * DirectionalLight::NUM_DATA;

            if (dirLights[i])
            {
                const auto &data = dirLights[i]->getData();
                for (uint32_t j = 0; j < DirectionalLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = data[j];
                }
            }
            else
            {
                for (uint32_t j = 0; j < DirectionalLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = 0.f;
                }
            }
        }

        offset += NUN_DIRLIGHTS_DATA;

        for (int i = 0; i < MAX_NR_POINTLIGHTS; i++)
        {
            const uint32_t localOffset = offset + i * PointLight::NUM_DATA;

            if (pointLights[i])
            {
                const auto &data = pointLights[i]->getData();
                for (uint32_t j = 0; j < PointLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = data[j];
                }
            }
            else
            {
                for (uint32_t j = 0; j < PointLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = 0.f;
                }
            }
        }

        offset += NUM_POINTLIGHTS_DATA;

        for (int i = 0; i < MAX_NR_SPOTLIGHTS; i++)
        {
            const uint32_t localOffset = offset + i * SpotLight::NUM_DATA;

            if (spotLights[i])
            {
                const auto &data = spotLights[i]->getData();
                for (uint32_t j = 0; j < SpotLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = data[j];
                }
            }
            else
            {
                for (uint32_t j = 0; j < SpotLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = 0.f;
                }
            }
        }

        shaderUniformbufferLights.setData(&lightData[0], LIGHT_DATA_SIZE, 0);
    }
    void render()
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0);
        updateLights();

        float shaderData[] = {
            Time::deltaTime,
            Time::time,
            activeWindow->width,
            activeWindow->height,
        };

        shaderUniformbufferInput.setData(&shaderData[0], sizeof(shaderData) * sizeof(float), sizeof(glm::vec3));

        for (const auto &camera : activeScene->getCameras())
        {
            camera->renderToTexture();
        }
        activeScene->mainCamera->renderToTexture();

        activeWindow->drawToWindow();
    }
}