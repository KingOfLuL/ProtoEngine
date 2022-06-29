layout (std140, binding = 2) uniform Values 
{
    vec3 _ViewPosition;
    float _DeltaTime;
    float _Time;
    float _ScreenWidth;
    float _ScreenHeight;
};

const float _Gamma = 2.2;