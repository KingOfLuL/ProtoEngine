#include "libs.hpp"

#include "entity.hpp"

#include "util/util.hpp"
#include "engine.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
    std::map<std::string, Entity *> loadedModels;

    Entity::Entity(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl) : name("NULL"), transform(pos, rot, scl)
    {
        transform.entity = this;
        activeScene->addEntity(this);
    }
    Entity::Entity(const Entity &entity)
    {
        name = entity.name;
        parent = entity.parent;
        transform = entity.transform;

        for (auto component : entity.m_Components)
        {
            auto c_camera = dynamic_cast<Camera *>(component);
            if (c_camera)
                addComponent(new Camera(*c_camera));

            auto c_light = dynamic_cast<Light *>(component);
            if (c_light)
                addComponent(new Light(*c_light));

            auto c_meshRenderer = dynamic_cast<MeshRenderer *>(component);
            if (c_meshRenderer)
                addComponent(new MeshRenderer(*c_meshRenderer));
            auto c_behavior = dynamic_cast<Behavior *>(component);
            if (c_behavior)
                addComponent(component);
        }
        for (auto c : entity.children)
            children.push_back(new Entity(*c));

        activeScene->addEntity(this);
    }
    Entity::~Entity()
    {
        activeScene->removeEntity(this);
        for (auto c : m_Components)
            delete c;

        for (auto c : children)
            delete c;
    }
    void Entity::forEachChildren(const std::function<void(Entity *)> &function)
    {
        function(this);
        for (auto child : children)
            child->forEachChildren(function);
    }
    Entity *Entity::getChildByName(const std::string &name) const
    {
        for (auto child : children)
            if (child->name == name)
                return child;
        return nullptr;
    }

    Entity *loadModel(const std::string &path, Shader *shader) // TODO: if meshes share same material, combine them
    {
        std::string filePath = PathUtil::FULL_PATH + PathUtil::MODEL_PATH + path;
        if (loadedModels.find(filePath) != loadedModels.end())
        {
            return new Entity(*loadedModels[filePath]);
        }

        Assimp::Importer importer;

        const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "Error (assimp) loading model: " << importer.GetErrorString() << std::endl;
        }
        aiNode *rootNode = scene->mRootNode;

        Entity *rootEntity = new Entity;
        Entity *entity = rootEntity;
        entity->name = rootNode->mName.C_Str();

        std::vector<Texture2D> loadedTextures;

        for (unsigned int i = 0; i < rootNode->mNumChildren; i++) // go through all (children) nodes
        {
            aiNode *node = rootNode->mChildren[i];
            for (unsigned int j = 0; j < node->mNumMeshes; j++) // mesh of (children) node
            {
                if (rootNode->mNumChildren > 1)
                {
                    entity = new Entity;
                    entity->parent = rootEntity;
                    rootEntity->children.push_back(entity);
                }

                aiMesh *mesh = scene->mMeshes[node->mMeshes[j]];
                entity->name = mesh->mName.C_Str();

                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;

                for (unsigned int k = 0; k < mesh->mNumVertices; k++) // vertices
                {
                    Vertex vertex;
                    glm::vec3 vector;

                    vector.x = mesh->mVertices[k].x;
                    vector.y = mesh->mVertices[k].y;
                    vector.z = mesh->mVertices[k].z;

                    vertex.position = vector;

                    if (mesh->HasNormals())
                    {
                        vector.x = mesh->mNormals[k].x;
                        vector.y = mesh->mNormals[k].y;
                        vector.z = mesh->mNormals[k].z;

                        vertex.normal = vector;
                    }
                    if (mesh->mTextureCoords[0])
                    {
                        vector.x = mesh->mTextureCoords[0][k].x;
                        vector.y = mesh->mTextureCoords[0][k].y;

                        vertex.uv.x = vector.x;
                        vertex.uv.y = vector.y;
                    }
                    if (mesh->HasVertexColors(0))
                    {
                        vertex.color.r = mesh->mColors[0][0].r;
                        vertex.color.g = mesh->mColors[0][0].g;
                        vertex.color.b = mesh->mColors[0][0].b;
                    }
                    else
                    {
                        vertex.color = glm::vec3(1.f);
                    }
                    vertices.push_back(vertex);
                }
                for (unsigned int k = 0; k < mesh->mNumFaces; k++) // indices
                {
                    aiFace face = mesh->mFaces[k];
                    for (unsigned int l = 0; l < face.mNumIndices; l++)
                        indices.push_back(face.mIndices[l]);
                }

                Mesh newMesh(vertices, indices);

                MeshRenderer *renderer = entity->addComponent<MeshRenderer>();
                renderer->setMesh(newMesh);

                // textures
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

                auto loadedIndex = Renderer::loadedMaterials.find(material->GetName().C_Str());
                if (loadedIndex != Renderer::loadedMaterials.end())
                {
                    renderer->material = Renderer::loadedMaterials[material->GetName().C_Str()];
                }
                else
                {
                    renderer->material = new Material(shader);

                    float shininess = 32.f;
                    material->Get(AI_MATKEY_SHININESS, shininess);
                    renderer->material->shininess = shininess;

                    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
                    {
                        loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE, renderer->material->textures, loadedTextures);
                        renderer->material->hasDiffuseTexture = true;
                    }
                    else
                    {
                        renderer->material->hasDiffuseTexture = false;
                        aiColor3D color;
                        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                        renderer->material->diffuseColor = {color.r, color.g, color.b};
                    }
                    if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
                    {
                        loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR, renderer->material->textures, loadedTextures);
                        renderer->material->hasSpecularTexture = true;
                    }
                    else
                    {
                        renderer->material->hasSpecularTexture = false;
                    }
                }
            }
        }
        if (boost::algorithm::ends_with(filePath, ".fbx"))
            rootEntity->transform.rotation = glm::vec3(-90, 0, 0);
        loadedModels.insert({filePath, rootEntity});

        return rootEntity;
    }
}