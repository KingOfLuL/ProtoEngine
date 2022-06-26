#include "libs.hpp"

#include "material.hpp"
#include "renderer/renderer.hpp"
#include "util/util.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Engine
{
    std::vector<Material *> Material::s_LoadedMaterials;

    Material::Material(const std::string &name) : name(name)
    {
        boost::property_tree::ptree mat;
        boost::property_tree::read_json(PathUtil::FULL_PATH + PathUtil::MATERIAL_PATH + name + ".mat.json", mat);

        int i = 0;
        for (auto &item : mat.get_child("diffuseColor"))
            diffuseColor[i++] = item.second.get_value<float>();

        twoSided = mat.get<bool>("twoSided");

        std::string diffusePath = mat.get<std::string>("diffuse");
        std::string specularPath = mat.get<std::string>("specular");

        if (diffusePath != "RenderTexture")
        {
            if (diffusePath != "")
                loadMaterialTexture(diffusePath, TextureType::DIFFUSE, this);
            else
                hasDiffuseTexture = false;
        }
        if (specularPath != "RenderTexture")
        {
            if (specularPath != "")
                loadMaterialTexture(specularPath, TextureType::SPECULAR, this);
            else if (specularPath != "RenderTexture")
                hasSpecularTexture = false;
        }

        shader = Shader::getShaderByName(mat.get<std::string>("shader"));

        s_LoadedMaterials.push_back(this);
    }
    Material *Material::getMaterialByName(const std::string &matName)
    {
        for (Material *mat : s_LoadedMaterials)
            if (mat->name == matName)
                return mat;
        return nullptr;
    }
    void Material::loadMaterialTexture(const std::string &path, TextureType texType, Material *material)
    {
        // check if texture already is loaded
        for (uint32_t i = 0; i < Texture2D::s_LoadedTextures.size(); i++)
        {
            // retrieve texture from list of loaded textures
            auto front = Texture2D::s_LoadedTextures.begin();
            std::advance(front, i);
            Texture2D *tex = &(*front);

            if (tex->getPath() == path)
            {
                material->textures.push_back(tex);
                return;
            }
        }
        // otherwise create a new one
        Texture2D tex = Texture2D::loadFromFile(path, texType);
        Texture2D::s_LoadedTextures.push_back(tex);
        material->textures.push_back(&(Texture2D::s_LoadedTextures.back()));
    }
}