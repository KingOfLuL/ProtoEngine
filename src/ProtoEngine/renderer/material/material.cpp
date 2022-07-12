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

        i32 i = 0;
        for (auto &item : mat.get_child("diffuseColor"))
            diffuseColor[i++] = item.second.get_value<f32>();

        twoSided = mat.get<bool>("twoSided");
        shininess = mat.get<f32>("shininess");

        std::string diffusePath = mat.get<std::string>("diffuse");
        std::string specularPath = mat.get<std::string>("specular");
        std::string normalPath = mat.get<std::string>("normal");

        if (diffusePath != "RenderTexture")
        {
            if (diffusePath != "")
                diffuseTex = loadMaterialTexture(diffusePath, TextureType::MAT_DIFFUSE);
            else
                hasDiffuseTexture = false;
        }
        if (specularPath != "RenderTexture")
        {
            if (specularPath != "")
                specularTex = loadMaterialTexture(specularPath, TextureType::MAT_SPECULAR);
            else if (specularPath != "RenderTexture")
                hasSpecularTexture = false;
        }
        if (normalPath != "RenderTexture")
        {
            if (normalPath != "")
                normalTex = loadMaterialTexture(normalPath, TextureType::MAT_NORMAL);
            else if (normalPath != "RenderTexture")
                hasNormalMap = false;
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
    Texture2D *Material::loadMaterialTexture(const std::string &path, TextureType texType)
    {
        // check if texture already is loaded
        for (u32 i = 0; i < Texture2D::s_LoadedTextures.size(); i++)
        {
            // retrieve texture from list of loaded textures
            auto front = Texture2D::s_LoadedTextures.begin();
            std::advance(front, i);
            Texture2D *tex = &(*front);

            if (tex->getPath() == path)
                return tex;
        }
        // otherwise create a new one
        Texture2D tex = Texture2D::loadFromFile(path, texType);
        Texture2D::s_LoadedTextures.push_back(tex);
        return &(Texture2D::s_LoadedTextures.back());
    }
}