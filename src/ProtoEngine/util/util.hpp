#ifndef SRC_ENGINE_UTIL_UTIL
#define SRC_ENGINE_UTIL_UTIL

#include "application/application.hpp"
#include "renderer/mesh/mesh.hpp"
#include "util/types.h"

namespace Engine
{
    class Listener
    {
    public:
        Listener() = default;
        Listener(std::function<void()> func);
        void call();

    private:
        std::function<void()> m_Function;
    };

    class Event
    {
    public:
        void subscribe(Listener *listener);
        void operator+=(Listener *listener);
        void unsubscribe(Listener *listener);
        void operator-=(Listener *listener);
        void call();

    private:
        std::vector<Listener *> m_Listeners;
    };

    void internal_util_init();

    namespace PathUtil
    {
        extern std::string FULL_PATH;
        const std::string SHADER_PATH = "/res/shaders/";
        const std::string SHADER_INCLUDE_PATH = "/res/shaders/Include/";
        const std::string MODEL_PATH = "/res/models/";
        const std::string TEXTURE_PATH = "/res/textures/";
        const std::string MATERIAL_PATH = "/res/materials/";

        std::string getRelativePath(std::string &path, const std::string &type);
        std::string toUnixStylePath(const std::string &path);
    }
    namespace GeomUtil
    {
        constexpr glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
        constexpr glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
        constexpr glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);

        /// corners given counter clockwise
        /// cornerA to cornerD front face, cornerE to cornerH backface
        std::array<Vertex, 36> createCubeVertices(
            const glm::vec3 &cornerA, const glm::vec3 &cornerB, const glm::vec3 &cornerC, const glm::vec3 &cornerD,
            const glm::vec3 &cornerE, const glm::vec3 &cornerF, const glm::vec3 &cornerG, const glm::vec3 &cornerH);
        /// corners given counter clockwise
        /// cornerA to cornerD front face, cornerE to cornerH backface
        std::array<Vertex, 48> createLineCubeVertices(
            const glm::vec3 &cornerA, const glm::vec3 &cornerB, const glm::vec3 &cornerC, const glm::vec3 &cornerD,
            const glm::vec3 &cornerE, const glm::vec3 &cornerF, const glm::vec3 &cornerG, const glm::vec3 &cornerH);
    }
    std::string loadTextFile(const std::string &path);
}
#endif // SRC_ENGINE_UTIL_UTIL
