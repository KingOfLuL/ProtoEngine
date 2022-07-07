struct DirLight // 64 bytes -> 64 bytes (16 floats) => 0 bytes padding
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    bool isActive;
};
struct PointLight // 76 bytes -> 80 bytes (20 floats) => 4 bytes padding (1 floats)
{
    vec3 position; 
    
    vec3 ambient;  
    vec3 diffuse;   
    vec3 specular; 

    float constant;
    float linear;   
    float quadratic;

    bool isActive;
};
struct SpotLight // 100 bytes -> 112 bytes (28 floats) => 12 bytes padding (3 floats)
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    bool isActive;
};

#define MAX_NR_DIRLIGHTS 2
#define MAX_NR_POINTLIGHTS 5
#define MAX_NR_SPOTLIGHTS 5

layout (std140, binding = 1) uniform Lights
{
    DirLight _DirLights[MAX_NR_DIRLIGHTS];
    PointLight _PointLights[MAX_NR_POINTLIGHTS];
    SpotLight _SpotLights[MAX_NR_SPOTLIGHTS];
};