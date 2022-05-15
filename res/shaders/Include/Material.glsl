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

void getTextureColors()
{
    if (_Material.hasDiffuse)
        _TextureDiffuseColor = texture(_Material.diffuseTexture, _Fragment.UV);
    else
        _TextureDiffuseColor = vec4(_Material.diffuseColor, 1.0);

    if (_Material.hasSpecular)
        _TextureSpecularColor = texture(_Material.specularTexture, _Fragment.UV);
    else
        _TextureSpecularColor = vec4(vec3(0.0), 1.0);
}