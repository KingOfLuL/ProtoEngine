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

    Entity *loadModel(const std::string &path) // TODO: make it faster
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

        if (rootNode->mNumChildren > 1)
        {
            entity = new Entity;
            entity->parent = rootEntity;
            rootEntity->children.push_back(entity);
        }

        for (unsigned int i = 0; i < rootNode->mNumChildren; i++) // go through all (children) nodes
        {
            aiNode *node = rootNode->mChildren[i];
            for (unsigned int j = 0; j < node->mNumMeshes; j++) // mesh of (children) node
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[j]];
                entity->name = mesh->mName.C_Str();

                std::vector<Vertex> vertices;
                std::vector<uint32_t> indices;

                for (unsigned int k = 0; k < mesh->mNumVertices; k++) // vertices
                {
                    Vertex vertex;

                    vertex.position.x = mesh->mVertices[k].x;
                    vertex.position.y = mesh->mVertices[k].y;
                    vertex.position.z = mesh->mVertices[k].z;

                    if (mesh->HasNormals())
                    {
                        vertex.normal.x = mesh->mNormals[k].x;
                        vertex.normal.y = mesh->mNormals[k].y;
                        vertex.normal.z = mesh->mNormals[k].z;
                    }
                    if (mesh->mTextureCoords[0])
                    {
                        vertex.uv.x = mesh->mTextureCoords[0][k].x;
                        vertex.uv.y = mesh->mTextureCoords[0][k].y;
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

                // Mesh newMesh(vertices, indices); // TODO: combining meshes with same material

                // MeshRenderer *renderer = entity->addComponent<MeshRenderer>();
                // renderer->setMesh(newMesh);

                // material
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

                std::string matName = material->GetName().C_Str();
                Material *mat = Material::getMaterialByName(matName);
                if (mat)
                {
                    auto renderer = entity->getComponent<MeshRenderer>();
                    renderer->addToMesh({vertices, indices});
                }
                else
                {
                    entity = new Entity;
                    entity->parent = rootEntity;
                    rootEntity->children.push_back(entity);

                    mat = new Material(matName);

                    auto renderer = entity->addComponent<MeshRenderer>();
                    renderer->setMesh({vertices, indices});
                    renderer->material = mat;
                }
            }
        }
        if (boost::algorithm::ends_with(filePath, ".fbx"))
            rootEntity->transform.rotation = glm::vec3(-90, 0, 0);
        loadedModels.insert({filePath, rootEntity});

        return rootEntity;
    }
}