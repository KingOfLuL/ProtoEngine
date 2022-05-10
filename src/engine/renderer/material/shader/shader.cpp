#include "libs.hpp"

#include "Shader.hpp"

#include "util/util.hpp"
#include "glad/glad.h"

namespace Engine
{
    Shader::Shader(const char *vertexPath, const char *fragmentPath)
    {
        m_VertexCode = loadTextFile(PathUtil::SHADER_PATH + vertexPath);
        m_FragmentCode = loadTextFile(PathUtil::SHADER_PATH + fragmentPath);

        m_VertexCode = processIncludes(m_VertexCode);
        m_FragmentCode = processIncludes(m_FragmentCode);

        const char *c_vCode = m_VertexCode.c_str(),
                   *c_fCode = m_FragmentCode.c_str();
        uint32_t vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &c_vCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX", vertexPath);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &c_fCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT", fragmentPath);

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);
        checkCompileErrors(m_ID, "PROGRAM", "");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    void Shader::addGeometryShader(const std::string &path)
    {
        m_GeometryCode = loadTextFile(PathUtil::SHADER_PATH + path);
        m_GeometryCode = processIncludes(m_GeometryCode);
        const char *c_code = m_GeometryCode.c_str();

        uint32_t id = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(id, 1, &c_code, NULL);
        glCompileShader(id);
        checkCompileErrors(id, "GEOMETRY", path);
        glAttachShader(m_ID, id);
        glLinkProgram(m_ID);

        glDeleteShader(id);
    }
    void Shader::use() const
    {
        glUseProgram(m_ID);
    }
    void Shader::checkCompileErrors(GLuint shader, const std::string &type, const std::string &path)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR: " << path << "  " << type << "\n"
                          << infoLog << std::endl;
                if (type == "FRAGMENT")
                    printCode(m_FragmentCode);
                else if (type == "VERTEX")
                    printCode(m_VertexCode);
                else if (type == "GEOMETRY")
                    printCode(m_GeometryCode);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR: " << type << "\n"
                          << infoLog << std::endl;
            }
        }
    }
    void Shader::setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
    }
    void Shader::setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    void Shader::setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
    }
    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
    }
    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    std::string Shader::processIncludes(const std::string &shader)
    {
        std::string newShader;
        std::stringstream ss(shader);
        std::string line;

        while (std::getline(ss, line, '\n'))
        {
            size_t startOfInclude = line.find("#include ");
            if (startOfInclude < line.size())
            {
                if (line.find("//") < startOfInclude) // #include is commented out
                    continue;

                size_t fileNameStartindex = strlen("#include ") + 1;
                size_t endOfInclude = line.find('"', startOfInclude + fileNameStartindex);

                std::string name = line.substr(startOfInclude + fileNameStartindex, endOfInclude - (startOfInclude + fileNameStartindex));
                std::string includedCode = loadTextFile(PathUtil::SHADER_INCLUDE_PATH + name);
                includedCode = processIncludes(includedCode);

                line.replace(line.begin() + startOfInclude, line.end(), includedCode);
            }
            newShader += line + '\n';
        }
        return newShader;
    }
    void Shader::printCode(const std::string &code) const
    {
        std::istringstream iss(code);
        int num = 1;
        for (std::string line; std::getline(iss, line); num++)
            std::cout << "(" << num << ") " << line << '\n';
        std::cout << "----------------------------------------------------------" << std::endl;
    }
}