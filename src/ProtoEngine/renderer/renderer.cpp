#include "libs.hpp"

#include "renderer.hpp"

#include "glad/glad.h"
#include "util/util.hpp"
#include "time/time.hpp"
#include "application/application.hpp"

namespace Engine::Renderer
{
    Uniformbuffer shaderUniformbufferMatrices;
    Uniformbuffer shaderUniformbufferLights;
    Uniformbuffer shaderUniformbufferInput;

    const u32 MATRIX_DATA_SIZE = 2 * sizeof(glm::mat4);

    const u32 NUN_DIRLIGHTS_DATA = DirectionalLight::NUM_DATA * MAX_NR_DIRLIGHTS;
    const u32 NUM_POINTLIGHTS_DATA = PointLight::NUM_DATA * MAX_NR_POINTLIGHTS;
    const u32 NUM_SPOTLIGHTS_DATA = SpotLight::NUM_DATA * MAX_NR_SPOTLIGHTS;

    const u32 LIGHT_DATA_SIZE = (NUN_DIRLIGHTS_DATA + NUM_POINTLIGHTS_DATA + NUM_SPOTLIGHTS_DATA) * sizeof(f32);

    const u32 INPUT_DATA_SIZE = 9 * sizeof(f32);

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

        shaderUniformbufferMatrices = Uniformbuffer(MATRIX_DATA_SIZE, 0);
        shaderUniformbufferLights = Uniformbuffer(LIGHT_DATA_SIZE, 1);
        shaderUniformbufferInput = Uniformbuffer(INPUT_DATA_SIZE, 2);
    }
    void updateLights()
    {
        const auto &dirLights = application->scene->getDirectionalLights();
        const auto &pointLights = application->scene->getPointLights();
        const auto &spotLights = application->scene->getSpotLights();

        std::array<f32, LIGHT_DATA_SIZE / sizeof(f32)> lightData;

        i32 offset = 0;

        for (i32 i = 0; i < MAX_NR_DIRLIGHTS; i++)
        {
            const u32 localOffset = offset + i * DirectionalLight::NUM_DATA;

            if (dirLights[i])
            {
                const auto &data = dirLights[i]->getData();
                for (u32 j = 0; j < DirectionalLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = data[j];
                }
            }
            else
            {
                for (u32 j = 0; j < DirectionalLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = 0.f;
                }
            }
        }

        offset += NUN_DIRLIGHTS_DATA;

        for (i32 i = 0; i < MAX_NR_POINTLIGHTS; i++)
        {
            const u32 localOffset = offset + i * PointLight::NUM_DATA;

            if (pointLights[i])
            {
                const auto &data = pointLights[i]->getData();
                for (u32 j = 0; j < PointLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = data[j];
                }
            }
            else
            {
                for (u32 j = 0; j < PointLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = 0.f;
                }
            }
        }

        offset += NUM_POINTLIGHTS_DATA;

        for (i32 i = 0; i < MAX_NR_SPOTLIGHTS; i++)
        {
            const u32 localOffset = offset + i * SpotLight::NUM_DATA;

            if (spotLights[i])
            {
                const auto &data = spotLights[i]->getData();
                for (u32 j = 0; j < SpotLight::NUM_DATA; j++)
                {
                    lightData[localOffset + j] = data[j];
                }
            }
            else
            {
                for (u32 j = 0; j < SpotLight::NUM_DATA; j++)
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

        f32 shaderData[] = {
            Time::deltaTime,
            Time::time,
            application->window->width,
            application->window->height,
        };

        shaderUniformbufferInput.setData(&shaderData[0], sizeof(shaderData), sizeof(glm::vec3));

        for (auto camera : application->scene->getCameras())
        {
            if (camera != application->scene->mainCamera)
                camera->renderToTexture();
        }
        application->scene->mainCamera->renderToTexture();

        application->window->drawToWindow(application->scene->mainCamera->renderTarget);
    }
}