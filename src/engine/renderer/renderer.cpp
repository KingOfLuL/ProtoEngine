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

    Shader shaderLit;

    std::map<std::string, Material *> loadedMaterials;

    Uniformbuffer shaderUniformbufferMatrices;
    Uniformbuffer shaderUniformbufferLights;
    Uniformbuffer shaderUniformbufferInput;
    const uint32_t matrixDataSize = 2 * sizeof(glm::mat4);
    const uint32_t lightDataSize =
        16 * sizeof(float) * MAX_NR_DIRLIGHTS +   /* directional lights */
        20 * sizeof(float) * MAX_NR_POINTLIGHTS + /* point light */
        24 * sizeof(float) * MAX_NR_SPOTLIGHTS;   /* spot light */
    const uint32_t numberLightsDataSize = 3 * sizeof(int);
    const uint32_t inputDataSize = 7 * sizeof(float);

    // TODO: add Batched Renderer

    void init()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // TODO: Add transparency -> sorting transparent meshes

        shaderLit = Shader("vertex/vertex.glsl.vs", "fragment/lit.glsl.fs");
        shaderUniformbufferMatrices = Uniformbuffer(matrixDataSize, 0);
        shaderUniformbufferLights = Uniformbuffer(lightDataSize + numberLightsDataSize, 1);
        shaderUniformbufferInput = Uniformbuffer(inputDataSize, 2);
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
        shaderUniformbufferLights.setData(&numberLights[0], numberLightsDataSize, lightDataSize);

        std::vector<float> lightData;
        for (int i = 0; i < MAX_NR_DIRLIGHTS; i++)
            if (dirLights[i])
            {
                auto data = dirLights[i]->getData();
                lightData.insert(lightData.end(), data.begin(), data.end());
            }
            else
                for (int j = 0; j < 16; j++)
                    lightData.push_back(0.f);
        for (int i = 0; i < MAX_NR_POINTLIGHTS; i++)
            if (pointLights[i])
            {
                auto data = pointLights[i]->getData();
                lightData.insert(lightData.end(), data.begin(), data.end());
            }
            else
                for (int j = 0; j < 20; j++)
                    lightData.push_back(0.f);
        for (int i = 0; i < MAX_NR_SPOTLIGHTS; i++)
            if (spotLights[i])
            {
                auto data = spotLights[i]->getData();
                lightData.insert(lightData.end(), data.begin(), data.end());
            }
            else
                for (int j = 0; j < 24; j++)
                    lightData.push_back(0.f);
        shaderUniformbufferLights.setData(&lightData[0], lightDataSize, 0);
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

        if (activeScene->skybox)
            activeScene->skybox->draw();

        for (auto renderer : activeScene->getRenderers())
        {
            Material *material = renderer->material;

            if (material->twoSided)
                glDisable(GL_CULL_FACE);
            else
                glEnable(GL_CULL_FACE);

            material->shader->use();

            material->shader->setMat4("_ModelMatrix", renderer->entity->transform.getTransformationMatrix());
            material->shader->setBool("_Material.hasTransparency", false);

            for (size_t i = 0; i < material->textures.size(); i++)
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

            glActiveTexture(GL_TEXTURE0);
            renderer->drawMesh();
        }

        activeScene->mainCamera->targetTexture.unbindFramebuffer();
        glClear(GL_COLOR_BUFFER_BIT);
        activeWindow->shader.use();
        activeWindow->screen.bind();
        activeScene->mainCamera->targetTexture.bindTexture();
        glDrawElements(GL_TRIANGLES, activeWindow->screen.getIndicesCount(), GL_UNSIGNED_INT, 0);
        activeWindow->screen.unbind();
    }
}