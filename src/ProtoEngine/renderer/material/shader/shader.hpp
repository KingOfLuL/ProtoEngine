#ifndef SRC_ENGINE_RENDERER_MATERIAL_SHADER_SHADER
#define SRC_ENGINE_RENDERER_MATERIAL_SHADER_SHADER

#include <string>
#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

namespace Engine
{
    class Shader
    {
    public:
        Shader() = default;
        Shader(const char *vertexPath, const char *fragmentPath, const std::string &name);

    public:
        void use() const;
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, i32 value) const;
        void setFloat(const std::string &name, f32 value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, f32 x, f32 y) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, f32 x, f32 y, f32 z) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        void addGeometryShader(const std::string &path);

        static Shader *getShaderByName(const std::string &name);

    public:
        std::string name;

    private:
        u32 m_ID;
        std::string m_VertexCode;
        std::string m_FragmentCode;
        std::string m_GeometryCode;

        static std::vector<Shader *> s_Shaders;

    private:
        void checkCompileErrors(u32 shader, const std::string &type, const std::string &path);
        std::string processIncludes(const std::string &shader);
        void printCode(const std::string &code) const;
    };
}
#endif // SRC_ENGINE_RENDERER_MATERIAL_SHADER_SHADER
