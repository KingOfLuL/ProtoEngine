#include "libs.hpp"

#include "material.hpp"
#include "renderer/renderer.hpp"
#include "util/util.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Engine
{
    std::vector<Material *> Material::s_LoadedMaterials;

    Material::Material(const std::string &name) : name(name) // FIXME: sometimes textures are not loaded
    {
        boost::property_tree::ptree mat;
        boost::property_tree::read_json(PathUtil::FULL_PATH + PathUtil::MATERIAL_PATH + name + ".mat.json", mat);

        std::string diffusePath = mat.get<std::string>("diffuse");
        if (diffusePath == "")
            hasDiffuseTexture = false;
        std::string specularPath = mat.get<std::string>("specular");
        if (specularPath == "")
            hasSpecularTexture = false;

        int i = 0;
        for (auto &item : mat.get_child("diffuseColor"))
            diffuseColor[i++] = item.second.get_value<float>();

        twoSided = mat.get<bool>("twoSided");

        loadMaterialTexture(diffusePath, TextureType::DIFFUSE, this);
        loadMaterialTexture(specularPath, TextureType::SPECULAR, this);

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
}