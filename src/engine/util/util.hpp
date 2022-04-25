#ifndef SRC_ENGINE_UTIL_UTIL
#define SRC_ENGINE_UTIL_UTIL

namespace Engine
{
    class Event
    {
    public:
        void subscribe(std::function<void(void)> func);
        void operator+=(std::function<void(void)> func);
        // void removeEvent(std::function<void(void)> func); // TODO: implement event removing
        // void operator-=(std::function<void(void)> func);
        void call();

    private:
        std::vector<std::function<void(void)>> m_Functions;
    };

    void internal_util_init();

    namespace PathUtil
    {
        extern std::string FULL_PATH;
        const std::string SHADER_PATH = "/res/shaders/";
        const std::string SHADER_INCLUDE_PATH = "/res/shaders/Include/";
        const std::string MODEL_PATH = "/res/models/";
        const std::string TEXTURE_PATH = "/res/textures/";
        const std::string DEFAULT_MESHES_PATH = "/res/models/Default/";

        std::string getRelativePath(std::string &path, const std::string &type);
        std::string toUnixStylePath(const std::string &path);
    }
    namespace GeomUtil
    {
        const glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
        const glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);
    }
    std::string loadTextFile(const std::string &path);
}
#endif // SRC_ENGINE_UTIL_UTIL
