layout (std140, binding = 2) uniform ShaderValues // 20 bytes (5 floats)
{
    vec3 _ViewPosition;
    float _DeltaTime;
    float _Time;
    float _ScreenWidth;
    float _ScreenHeight;
};