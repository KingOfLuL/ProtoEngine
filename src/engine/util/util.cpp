#include "libs.hpp"

#include "util.hpp"

#include "random/random.hpp"

namespace Engine
{
    void Event::subscribe(std::function<void(void)> func)
    {
        m_Functions.push_back(func);
    }
    void Event::operator+=(std::function<void(void)> func)
    {
        subscribe(func);
    }
    void Event::call()
    {
        for (auto func : m_Functions)
        {
            func();
        }
    }

    void internal_util_init()
    {
        using namespace PathUtil;

        Random::internal_init();

        FULL_PATH = std::filesystem::current_path().string();
        std::replace(FULL_PATH.begin(), FULL_PATH.end(), char(92), '/');
    }

    namespace PathUtil
    {
        std::string FULL_PATH;

        std::string getRelativePath(std::string &path, const std::string &type)
        {
            size_t index = path.find(type);
            return path.substr(index + type.length());
        }
        std::string toUnixStylePath(const std::string &path)
        {
            std::string str = path;
            std::replace(str.begin(), str.end(), char(92), '/'); // char(92) = '\'
            std::regex r("//");
            str = std::regex_replace(str, r, "/");
            return str;
        }
    }

    std::string loadTextFile(const std::string &path)
    {
        std::string text;
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            file.open(PathUtil::FULL_PATH + path);
            std::stringstream stream;

            stream << file.rdbuf();

            file.close();

            text = stream.str();
        }
        catch (const std::ifstream::failure &e)
        {
            std::cout << "File (" << path << ") not successfully read: " << e.what() << std::endl;
        }
        return text;
    }
    namespace GeomUtil
    {
        std::array<Vertex, 36> createCubeVertices(
            const glm::vec3 &cornerA, const glm::vec3 &cornerB, const glm::vec3 &cornerC, const glm::vec3 &cornerD,
            const glm::vec3 &cornerE, const glm::vec3 &cornerF, const glm::vec3 &cornerG, const glm::vec3 &cornerH)
        {
            return {
                // front
                Vertex(cornerA, {0, 0, -1}, {0, 1}), // a
                Vertex(cornerB, {0, 0, -1}, {0, 0}), // b
                Vertex(cornerC, {0, 0, -1}, {1, 0}), // c
                Vertex(cornerC, {0, 0, -1}, {0, 1}), // c
                Vertex(cornerD, {0, 0, -1}, {1, 0}), // d
                Vertex(cornerA, {0, 0, -1}, {1, 1}), // a

                // right
                Vertex(cornerD, {1, 0, 0}, {0, 1}), // a
                Vertex(cornerC, {1, 0, 0}, {0, 0}), // b
                Vertex(cornerG, {1, 0, 0}, {1, 0}), // c
                Vertex(cornerG, {1, 0, 0}, {0, 1}), // c
                Vertex(cornerH, {1, 0, 0}, {1, 0}), // d
                Vertex(cornerD, {1, 0, 0}, {1, 1}), // a

                // left
                Vertex(cornerE, {-1, 0, 0}, {0, 1}), // a
                Vertex(cornerF, {-1, 0, 0}, {0, 0}), // b
                Vertex(cornerB, {-1, 0, 0}, {1, 0}), // c
                Vertex(cornerB, {-1, 0, 0}, {0, 1}), // c
                Vertex(cornerA, {-1, 0, 0}, {1, 0}), // d
                Vertex(cornerE, {-1, 0, 0}, {1, 1}), // a

                // back
                Vertex(cornerH, {0, 0, 1}, {0, 1}), // a
                Vertex(cornerG, {0, 0, 1}, {0, 0}), // b
                Vertex(cornerF, {0, 0, 1}, {1, 0}), // c
                Vertex(cornerF, {0, 0, 1}, {0, 1}), // c
                Vertex(cornerE, {0, 0, 1}, {1, 0}), // d
                Vertex(cornerH, {0, 0, 1}, {1, 1}), // a

                // top
                Vertex(cornerE, {0, 1, 0}, {0, 1}), // a
                Vertex(cornerA, {0, 1, 0}, {0, 0}), // b
                Vertex(cornerD, {0, 1, 0}, {1, 0}), // c
                Vertex(cornerD, {0, 1, 0}, {0, 1}), // c
                Vertex(cornerH, {0, 1, 0}, {1, 0}), // d
                Vertex(cornerE, {0, 1, 0}, {1, 1}), // a

                // bottom
                Vertex(cornerB, {0, -1, 0}, {0, 1}), // a
                Vertex(cornerF, {0, -1, 0}, {0, 0}), // b
                Vertex(cornerG, {0, -1, 0}, {1, 0}), // c
                Vertex(cornerG, {0, -1, 0}, {0, 1}), // c
                Vertex(cornerC, {0, -1, 0}, {1, 0}), // d
                Vertex(cornerB, {0, -1, 0}, {1, 1}), // a
            };
        }
        std::array<Vertex, 48> createLineCubeVertices(
            const glm::vec3 &cornerA, const glm::vec3 &cornerB, const glm::vec3 &cornerC, const glm::vec3 &cornerD,
            const glm::vec3 &cornerE, const glm::vec3 &cornerF, const glm::vec3 &cornerG, const glm::vec3 &cornerH)
        {
            return {
                // front
                Vertex(cornerA, {0, 0, -1}, {0, 1}), // a
                Vertex(cornerB, {0, 0, -1}, {0, 0}), // b
                Vertex(cornerB, {0, 0, -1}, {0, 0}), // b
                Vertex(cornerC, {0, 0, -1}, {0, 1}), // c
                Vertex(cornerC, {0, 0, -1}, {0, 1}), // c
                Vertex(cornerD, {0, 0, -1}, {1, 0}), // d
                Vertex(cornerD, {0, 0, -1}, {1, 0}), // d
                Vertex(cornerA, {0, 0, -1}, {1, 1}), // a

                // back
                Vertex(cornerH, {0, 0, 1}, {0, 1}), // a
                Vertex(cornerG, {0, 0, 1}, {0, 0}), // b
                Vertex(cornerG, {0, 0, 1}, {0, 0}), // b
                Vertex(cornerF, {0, 0, 1}, {1, 0}), // c
                Vertex(cornerF, {0, 0, 1}, {0, 1}), // c
                Vertex(cornerE, {0, 0, 1}, {1, 0}), // d
                Vertex(cornerE, {0, 0, 1}, {1, 0}), // d
                Vertex(cornerH, {0, 0, 1}, {1, 1}), // a

                // right
                Vertex(cornerD, {1, 0, 0}, {0, 1}), // a
                Vertex(cornerC, {1, 0, 0}, {0, 0}), // b
                Vertex(cornerC, {1, 0, 0}, {0, 0}), // b
                Vertex(cornerG, {1, 0, 0}, {1, 0}), // c
                Vertex(cornerG, {1, 0, 0}, {0, 1}), // c
                Vertex(cornerH, {1, 0, 0}, {1, 0}), // d
                Vertex(cornerH, {1, 0, 0}, {1, 0}), // d
                Vertex(cornerD, {1, 0, 0}, {1, 1}), // a

                // left
                Vertex(cornerE, {-1, 0, 0}, {0, 1}), // a
                Vertex(cornerF, {-1, 0, 0}, {0, 0}), // b
                Vertex(cornerF, {-1, 0, 0}, {0, 0}), // b
                Vertex(cornerB, {-1, 0, 0}, {1, 0}), // c
                Vertex(cornerB, {-1, 0, 0}, {0, 1}), // c
                Vertex(cornerA, {-1, 0, 0}, {1, 0}), // d
                Vertex(cornerA, {-1, 0, 0}, {1, 0}), // d
                Vertex(cornerE, {-1, 0, 0}, {1, 1}), // a

                // top
                Vertex(cornerE, {0, 1, 0}, {0, 1}), // a
                Vertex(cornerA, {0, 1, 0}, {0, 0}), // b
                Vertex(cornerA, {0, 1, 0}, {0, 0}), // b
                Vertex(cornerD, {0, 1, 0}, {1, 0}), // c
                Vertex(cornerD, {0, 1, 0}, {0, 1}), // c
                Vertex(cornerH, {0, 1, 0}, {1, 0}), // d
                Vertex(cornerH, {0, 1, 0}, {1, 0}), // d
                Vertex(cornerE, {0, 1, 0}, {1, 1}), // a

                // bottom
                Vertex(cornerB, {0, -1, 0}, {0, 1}), // a
                Vertex(cornerF, {0, -1, 0}, {0, 0}), // b
                Vertex(cornerF, {0, -1, 0}, {0, 0}), // b
                Vertex(cornerG, {0, -1, 0}, {1, 0}), // c
                Vertex(cornerG, {0, -1, 0}, {0, 1}), // c
                Vertex(cornerC, {0, -1, 0}, {1, 0}), // d
                Vertex(cornerC, {0, -1, 0}, {1, 0}), // d
                Vertex(cornerB, {0, -1, 0}, {1, 1}), // a
            };
        }
    }
}