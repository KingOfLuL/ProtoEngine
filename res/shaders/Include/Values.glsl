layout (std140, binding = 2) uniform Values 
{
    vec3 _ViewPosition;
    float _DeltaTime;
    float _Time;
    float _ScreenWidth;
    float _ScreenHeight;
    float _NearClipPlane;
    float _FarClipPlane;
};

const float _Gamma = 2.2;