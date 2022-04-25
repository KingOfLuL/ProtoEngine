#include "Random.hpp"

namespace Engine::Random
{
    std::random_device rd;
    std::mt19937 mt;

    std::uniform_real_distribution<float> floatDist(0, 1);
    std::uniform_real_distribution<double> doubleDist(0, 1);
    std::uniform_int_distribution<int> intDist(0, 10);

    typedef std::uniform_real_distribution<float>::param_type floatParamType;
    typedef std::uniform_real_distribution<double>::param_type doubleParamType;
    typedef std::uniform_int_distribution<int>::param_type intParamType;

    void internal_init()
    {
        mt = std::mt19937(rd());
    }

    float float_(float min, float max)
    {
        floatParamType t(min, max);
        floatDist.param(t);
        return floatDist(mt);
    }
    double double_(double min, double max)
    {
        doubleParamType t(min, max);
        doubleDist.param(t);
        return doubleDist(mt);
    }
    int int_(int min, int max)
    {
        intParamType t(min, max);
        intDist.param(t);
        return intDist(mt);
    }
}