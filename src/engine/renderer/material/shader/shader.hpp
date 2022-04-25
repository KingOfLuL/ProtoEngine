#ifndef SRC_ENGINE_RENDERER_MATERIAL_SHADER_SHADER
#define SRC_ENGINE_RENDERER_MATERIAL_SHADER_SHADER

#include <string>
#include <glm/glm.hpp>
#include <cstdint>

namespace Engine
{
    class Shader
    {
    public:
        Shader() = default;
        Shader(const char *vertexPath, const char *fragmentPath);
        void use() const;
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        void addGeometryShader(const std::string &path);

    private:
        uint32_t m_ID;
        std::string m_VertexCode;
        std::string m_FragmentCode;
        std::string m_GeometryCode;

    private:
        void checkCompileErrors(uint32_t shader, const std::string &type, const std::string &path);
        std::string processIncludes(const std::string &shader);
        void printCode(const std::string &code) const;
    };
}
#endif // SRC_ENGINE_RENDERER_MATERIAL_SHADER_SHADER
