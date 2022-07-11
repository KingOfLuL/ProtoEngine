layout (std140, binding = 2) uniform Values 
{
    mat4 _LightSpaceMatrix;
    vec3 _ShadowCasterLightDirection;
    float _DeltaTime;
    float _Time;
    float _ScreenWidth;
    float _ScreenHeight;
    float _ShadowBiasMin;
    float _ShadowBiasMax;
};

uniform float _NearClipPlane;
uniform float _FarClipPlane;
uniform vec3 _ViewPosition;

const float _Gamma = 2.2;