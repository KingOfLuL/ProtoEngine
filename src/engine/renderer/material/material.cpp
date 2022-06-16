#include "libs.hpp"

#include "material.hpp"
#include "renderer/renderer.hpp"
#include "util/util.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Engine
{
    std::vector<Material *> Material::s_LoadedMaterials;

    Material::Material(const std::string &name) : name(name), hasDiffuseTexture(true), hasSpecularTexture(true)
    {
        boost::property_tree::ptree mat;
        boost::property_tree::read_json(PathUtil::FULL_PATH + PathUtil::MATERIAL_PATH + name + ".mat.json", mat);

        int i = 0;
        for (auto &item : mat.get_child("diffuseColor"))
            diffuseColor[i++] = item.second.get_value<float>();

        twoSided = mat.get<bool>("twoSided");

        std::string diffusePath = mat.get<std::string>("diffuse");
        std::string specularPath = mat.get<std::string>("specular");

        if (diffusePath != "")
            loadMaterialTexture(diffusePath, TextureType::DIFFUSE, this);
        else
            hasDiffuseTexture = false;
        if (specularPath != "")
            loadMaterialTexture(specularPath, TextureType::SPECULAR, this);
        else
            hasSpecularTexture = false;

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
        for (Texture2D tex : loadedTextures)
            if (tex.getPath() == path)
            {
                material->textures.push_back(tex);
                return;
            }
        Texture2D tex = Texture2D::loadFromFile(path, texType);
        material->textures.push_back(tex);
        loadedTextures.push_back(tex);
    }
}