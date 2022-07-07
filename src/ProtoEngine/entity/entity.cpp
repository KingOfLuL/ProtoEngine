#include "libs.hpp"

#include "entity.hpp"

#include "util/util.hpp"
#include "application/application.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
    std::map<std::string, Entity::Model *> Entity::s_LoadedModels;

    Entity::Entity(const std::string &name, const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl) : name(name), transform(pos, rot, scl)
    {
        transform.entity = this;
        application->scene->addEntity(this);
    }
    Entity::Entity(const Entity &entity)
    {
        application->scene->addEntity(this);

        name = entity.name;
        parent = entity.parent;
        transform = entity.transform;
        transform.entity = this;

        for (auto component : entity.m_Components)
        {
            auto c_camera = dynamic_cast<Camera *>(component);
            if (c_camera)
                addComponent<Camera>(*c_camera);

            auto c_dirLight = dynamic_cast<DirectionalLight *>(component);
            if (c_dirLight)
                addComponent<DirectionalLight>(*c_dirLight);

            auto c_PointLight = dynamic_cast<PointLight *>(component);
            if (c_PointLight)
                addComponent<PointLight>(*c_PointLight);

            auto c_SpotLight = dynamic_cast<SpotLight *>(component);
            if (c_SpotLight)
                addComponent<SpotLight>(*c_SpotLight);

            auto c_meshRenderer = dynamic_cast<MeshRenderer *>(component);
            if (c_meshRenderer)
                addComponent<MeshRenderer>(*c_meshRenderer);

            auto c_behavior = dynamic_cast<Behavior *>(component);
            if (c_behavior)
                addComponent<Behavior>(*c_behavior);
        }
        for (auto c : entity.children)
        {
            Entity *newChild = new Entity(*c);
            newChild->parent = this;
            children.push_back(newChild);
        }
    }
    Entity::Entity(Model *model)
        : name(model->entityName)
    {
        application->scene->addEntity(this);
        transform.entity = this;

        if (model->hasRenderer)
        {
            auto renderer = addComponent<MeshRenderer>();
            renderer->setMesh(model->mesh);
            renderer->material = model->material;
        }
        for (auto child : model->children)
        {
            Entity *ent_child = new Entity(child);
            ent_child->setParent(this);
        }
    }
    Entity::~Entity()
    {
        application->scene->removeEntity(this);
        for (auto c : m_Components)
            delete c;
        setParent(nullptr);
        for (auto c : children)
            delete c;
    }
    void Entity::setParent(Entity *parent)
    {
        if ((parent == nullptr || parent == NULL))
        {
            if (this->parent != nullptr)
            {
                auto it = std::find(this->parent->children.begin(), this->parent->children.end(), this);
                if (it != this->parent->children.end())
                    this->parent->children.erase(it);
                this->parent = nullptr;
            }
        }
        else
        {
            this->parent = parent;
            parent->children.push_back(this);
        }
    }
    void Entity::forEachChildren(const std::function<void(Entity *)> &function, bool b)
    {
        if (b)
            function(this);
        for (auto child : children)
            child->forEachChildren(function, false);
    }
    Entity *Entity::getChildByName(const std::string &name) const
    {
        for (auto child : children)
            if (child->name == name)
                return child;
        return nullptr;
    }

    Entity *Entity::loadModel(const std::string &path)
    {
        std::string filePath = PathUtil::FULL_PATH + PathUtil::MODEL_PATH + path;
        if (s_LoadedModels.find(filePath) != s_LoadedModels.end())
        {
            std::cout << "Already Loaded" << std::endl;
            Entity *ent = new Entity(s_LoadedModels.at(filePath));
            if (boost::algorithm::ends_with(filePath, ".fbx"))
                ent->transform.rotation.x = -90;
            return ent;
        }

        Assimp::Importer importer;

        const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "Error (assimp) loading model: " << importer.GetErrorString() << std::endl;
        }
        aiNode *rootNode = scene->mRootNode;

        Entity *rootEntity = new Entity(rootNode->mName.C_Str());
        Entity *entity = rootEntity;
        entity->name = rootNode->mName.C_Str();

        if (rootNode->mNumChildren > 1)
        {
            entity = new Entity(rootNode->mName.C_Str());
            entity->parent = rootEntity;
            rootEntity->children.push_back(entity);
        }

        for (u32 i = 0; i < rootNode->mNumChildren; i++) // go through all (children) nodes
        {
            aiNode *node = rootNode->mChildren[i];
            for (u32 j = 0; j < node->mNumMeshes; j++) // mesh of (children) node
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[j]];
                entity->name = mesh->mName.C_Str();

                std::vector<Vertex> vertices;
                std::vector<u32> indices;

                for (u32 k = 0; k < mesh->mNumVertices; k++) // vertices
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

                    vertex.tangent.x = mesh->mTangents[i].x;
                    vertex.tangent.y = mesh->mTangents[i].y;
                    vertex.tangent.z = mesh->mTangents[i].z;

                    vertex.bitangent.x = mesh->mBitangents[i].x;
                    vertex.bitangent.y = mesh->mBitangents[i].y;
                    vertex.bitangent.z = mesh->mBitangents[i].z;

                    vertices.push_back(vertex);
                }
                for (u32 k = 0; k < mesh->mNumFaces; k++) // indices
                {
                    aiFace face = mesh->mFaces[k];
                    for (u32 l = 0; l < face.mNumIndices; l++)
                        indices.push_back(face.mIndices[l]);
                }

                // material
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

                std::string matName = material->GetName().C_Str();

                entity = new Entity(node->mName.C_Str());
                entity->parent = rootEntity;
                rootEntity->children.push_back(entity);

                Material *mat = Material::getMaterialByName(matName); // TODO: clean up; combining meshes?
                if (mat)
                {
                    auto renderer = entity->getComponent<MeshRenderer>();
                    if (renderer)
                    {
                        renderer->addToMesh(vertices, indices);
                    }
                    else
                    {
                        renderer = entity->addComponent<MeshRenderer>();
                        renderer->setMesh(vertices, indices);
                        renderer->material = mat;
                    }
                }
                else
                {
                    mat = new Material(matName);

                    auto renderer = entity->addComponent<MeshRenderer>();
                    renderer->setMesh(vertices, indices);
                    renderer->material = mat;
                }
            }
        }
        if (boost::algorithm::ends_with(filePath, ".fbx"))
            rootEntity->transform.rotation.x = -90;

        Model *model = new Model(rootEntity, filePath);
        s_LoadedModels.insert(std::pair<std::string, Model *>(filePath, model));

        return rootEntity;
    }

    Entity::Model::Model(Entity *entity, const std::string &filePath)
        : fileName(filePath), entityName(entity->name)
    {
        auto renderer = entity->getComponent<MeshRenderer>();
        if (renderer != nullptr)
        {
            hasRenderer = true;
            mesh = renderer->mesh;
            material = renderer->material;
        }
        else
        {
            hasRenderer = false;
        }

        for (auto child : entity->children)
        {
            Model *model_child = new Model(child, fileName);
            model_child->parent = this;
            children.push_back(model_child);
        }
    }
}