#include "libs.hpp"

#include "renderer.hpp"

#include "engine.hpp"
#include "glad/glad.h"
#include "util/util.hpp"
#include "util/vertices.hpp"
#include "time/time.hpp"

#include <stb/stb_image.h>

namespace Engine::Renderer
{
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    Shader *shaderLit;
    Shader *shaderBounds;

    Uniformbuffer shaderUniformbufferMatrices;
    Uniformbuffer shaderUniformbufferLights;
    Uniformbuffer shaderUniformbufferInput;

    const uint32_t MATRIX_DATA_SIZE = 2 * sizeof(glm::mat4);

    const uint32_t DIRLIGHTS_DATA_SIZE = DirectionalLight::dataSize * MAX_NR_DIRLIGHTS;
    const uint32_t POINTLIGHTS_DATA_SIZE = PointLight::dataSize * MAX_NR_POINTLIGHTS;
    const uint32_t SPOTLIGHTS_DATA_SIZE = SpotLight::dataSize * MAX_NR_SPOTLIGHTS;

    const uint32_t LIGHT_DATA_SIZE = DIRLIGHTS_DATA_SIZE + POINTLIGHTS_DATA_SIZE + SPOTLIGHTS_DATA_SIZE;
    const uint32_t NUMBER_LIGHTS_DATA_SIZE = 3 * sizeof(int);

    const uint32_t INPUT_DATA_SIZE = 7 * sizeof(float);

    // TODO: add Batched Renderer

    void init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.0f, -1.0f);

        shaderLit = new Shader("vertex/vertex.vs.glsl", "fragment/lit.fs.glsl", "Lit");
        shaderBounds = new Shader("vertex/vertex.vs.glsl", "fragment/bounds.fs.glsl", "Bounds");
        shaderBounds->addGeometryShader("geometry/bounds.gs.glsl");

        shaderUniformbufferMatrices = Uniformbuffer(MATRIX_DATA_SIZE, 0);
        shaderUniformbufferLights = Uniformbuffer(LIGHT_DATA_SIZE + NUMBER_LIGHTS_DATA_SIZE, 1);
        shaderUniformbufferInput = Uniformbuffer(INPUT_DATA_SIZE, 2);
    }
    void updateLights()
    {
        auto dirLights = activeScene->getDirectionalLights();
        auto pointLights = activeScene->getPointLights();
        auto spotLights = activeScene->getSpotLights();

        int numberLights[3] = {0, 0, 0}; // [0] -> numDirLights, [1] -> numPointLights, [2] -> numSpotLights

        for (auto i : dirLights)
            if (i != nullptr)
                numberLights[0]++;
        for (auto i : pointLights)
            if (i != nullptr)
                numberLights[1]++;
        for (auto i : spotLights)
            if (i != nullptr)
                numberLights[2]++;
        shaderUniformbufferLights.setData(&numberLights[0], NUMBER_LIGHTS_DATA_SIZE, LIGHT_DATA_SIZE);

        std::array<float, LIGHT_DATA_SIZE> lightData;
        int offset = 0;

        for (int i = 0; i < MAX_NR_DIRLIGHTS; i++)
        {
            const uint32_t localOffset = offset + i * DirectionalLight::dataSize;

            if (dirLights[i])
            {
                const auto &data = dirLights[i]->getData();
                for (uint32_t j = 0; j < DirectionalLight::dataSize; j++)
                    lightData[localOffset + j] = data[j];
            }
            else
                for (uint32_t j = 0; j < DirectionalLight::dataSize; j++)
                    lightData[localOffset + j] = 0.f;
        }

        offset += DIRLIGHTS_DATA_SIZE;

        for (int i = 0; i < MAX_NR_POINTLIGHTS; i++)
        {
            const uint32_t localOffset = offset + i * PointLight::dataSize;

            if (pointLights[i])
            {
                const auto &data = pointLights[i]->getData();
                for (uint32_t j = 0; j < PointLight::dataSize; j++)
                    lightData[localOffset + j] = data[j];
            }
            else
                for (uint32_t j = 0; j < PointLight::dataSize; j++)
                    lightData[localOffset + j] = 0.f;
        }

        offset += POINTLIGHTS_DATA_SIZE;

        for (int i = 0; i < MAX_NR_SPOTLIGHTS; i++)
        {
            const uint32_t localOffset = offset + i * SpotLight::dataSize;

            if (spotLights[i])
            {
                const auto &data = spotLights[i]->getData();
                for (uint32_t j = 0; j < SpotLight::dataSize; j++)
                    lightData[localOffset + j] = data[j];
            }
            else
                for (uint32_t j = 0; j < SpotLight::dataSize; j++)
                    lightData[localOffset + j] = 0.f;
        }

        shaderUniformbufferLights.setData(&lightData[0], LIGHT_DATA_SIZE, 0);
    }
    void render()
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0);
        activeScene->mainCamera->targetTexture.bindFramebuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projectionMatrix = activeScene->mainCamera->getProjectionMatrix();
        viewMatrix = activeScene->mainCamera->getViewMatrix();

        shaderUniformbufferMatrices.setData(glm::value_ptr(viewMatrix), sizeof(glm::mat4), 0);
        shaderUniformbufferMatrices.setData(glm::value_ptr(projectionMatrix), sizeof(glm::mat4), sizeof(glm::mat4));

        updateLights();

        shaderUniformbufferInput.setData(glm::value_ptr(activeScene->mainCamera->entity->transform.position), sizeof(glm::vec3), 0);
        shaderUniformbufferInput.setData(&Time::deltaTime, sizeof(float), sizeof(glm::vec3));
        shaderUniformbufferInput.setData(&Time::time, sizeof(float), 4 * sizeof(float));
        shaderUniformbufferInput.setData(&activeWindow->width, sizeof(float), 5 * sizeof(float));
        shaderUniformbufferInput.setData(&activeWindow->height, sizeof(float), 6 * sizeof(float));

        glActiveTexture(GL_TEXTURE0);
        if (activeScene->skybox)
            activeScene->skybox->draw();

        const auto &renderers = activeScene->getRenderers();
        for (const auto &renderer : renderers)
        {
            Material *material = renderer->material;

            if (material->twoSided)
                glDisable(GL_CULL_FACE);
            else
                glEnable(GL_CULL_FACE);

            material->shader->use();

            material->shader->setMat4("_ModelMatrix", renderer->entity->transform.getTransformationMatrix());
            material->shader->setBool("_Material.hasTransparency", false);

            for (uint32_t i = 0; i < material->textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);

                if (material->textures[i].getType() == TextureType::DIFFUSE)
                    material->shader->setInt("_Material.diffuseTexture", i);
                else if (material->textures[i].getType() == TextureType::SPECULAR)
                    material->shader->setInt("_Material.specularTexture", i);
                if (material->textures[i].colorFormat == GL_RGBA)
                    material->shader->setBool("_Material.hasTransparency", true);

                material->textures[i].bind();
            }

            material->shader->setVec3("_Material.diffuseColor", material->diffuseColor);
            material->shader->setFloat("_Material.shininess", 32.f);

            material->shader->setBool("_Material.hasDiffuse", material->hasDiffuseTexture);
            material->shader->setBool("_Material.hasSpecular", material->hasSpecularTexture);

            renderer->drawMesh();
        }

        // TODO: clean that up
        activeScene->mainCamera->targetTexture.unbindFramebuffer();
        glClear(GL_COLOR_BUFFER_BIT);
        activeWindow->shader->use();
        activeWindow->screen.bind();
        activeScene->mainCamera->targetTexture.bindTexture();
        glDrawElements(GL_TRIANGLES, activeWindow->screen.getIndicesCount(), GL_UNSIGNED_INT, 0);
        activeWindow->screen.unbind();
    }
}