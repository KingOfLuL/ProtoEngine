struct Material 
{
    sampler2D diffuseTexture;
    bool hasDiffuse;
    vec3 diffuseColor;

    sampler2D specularTexture;
    bool hasSpecular;

    float shininess;

    bool hasTransparency;
};
uniform Material _Material;

vec4 _TextureDiffuseColor;
vec4 _TextureSpecularColor;

void getTextureColors(Material material, Fragment frag)
{
    if (material.hasDiffuse)
        _TextureDiffuseColor = texture(material.diffuseTexture, frag.UV);
    else
        _TextureDiffuseColor = vec4(material.diffuseColor, 1.0);

    if (material.hasSpecular)
        _TextureSpecularColor = texture(material.specularTexture, frag.UV);
    else
        _TextureSpecularColor = vec4(vec3(0.0), 1.0);
}