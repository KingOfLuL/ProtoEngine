vec4 calcReflection(Fragment frag)
{
    vec3 I = normalize(frag.Position - _ViewPosition);
    vec3 R = reflect(I, normalize(frag.Normal));

    return texture(_Skybox, R);
}
vec4 calcRefraction(Fragment frag, float reflectiveIndex)
{
    float ratio = 1.0 / reflectiveIndex;

    vec3 I = normalize(frag.Position - _ViewPosition);
    vec3 R = refract(I, normalize(frag.Normal), ratio);

    return texture(_Skybox, R);
}