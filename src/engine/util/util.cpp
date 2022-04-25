#include "libs.hpp"

#include "util.hpp"

#include "random/random.hpp"
#include <regex>

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
}