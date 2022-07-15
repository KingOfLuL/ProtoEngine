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
        for (auto &item : mat.get_child("DiffuseColor"))
            diffuseColor[i++] = item.second.get_value<f32>();

        twoSided = mat.get<bool>("TwoSided");
        shininess = mat.get<f32>("Shininess");

        for (const auto &[name, texture] : mat.get_child("Textures"))
        {
            const std::string &path = texture.get_value<std::string>();
            if (path != "")
            {
                Texture2D *tex = loadMaterialTexture(path);
                tex->name = name;
                textures.push_back(tex);
            }
        }

        shader = Shader::getShaderByName(mat.get<std::string>("Shader"));

        s_LoadedMaterials.push_back(this);
    }
    Material *Material::getMaterialByName(const std::string &matName)
    {
        for (Material *mat : s_LoadedMaterials)
            if (mat->name == matName)
                return mat;
        return nullptr;
    }
    Texture2D *Material::loadMaterialTexture(const std::string &path)
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
        Texture2D tex = Texture2D::loadFromFile(path, TextureType::MAT_DIFFUSE);
        Texture2D::s_LoadedTextures.push_back(tex);
        return &(Texture2D::s_LoadedTextures.back());
    }
}