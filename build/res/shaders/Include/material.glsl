struct Material 
{
    sampler2D DiffuseTexture;
    bool HasDiffuse;
    
    vec3 DiffuseColor;

    sampler2D SpecularTexture;
    bool HasSpecular;

    sampler2D NormalMap;
    bool HasNormal;

    float Shininess;
};
uniform Material _Material;

vec4 _TextureDiffuseColor;
float _TextureSpecularStrength;

void getTextureColors()
{
    if (_Material.HasDiffuse)
        _TextureDiffuseColor = texture2D(_Material.DiffuseTexture, _Fragment.UV) * vec4(_Material.DiffuseColor, 1.0);
    else
        _TextureDiffuseColor = vec4(_Material.DiffuseColor, 1.0);

    if (_Material.HasSpecular)
        _TextureSpecularStrength = texture2D(_Material.SpecularTexture, _Fragment.UV).r;
    else
        _TextureSpecularStrength = 1.0;
}