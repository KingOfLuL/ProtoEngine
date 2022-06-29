#include "libs.hpp"
#include "Random.hpp"

#include <random>

namespace Engine::Random
{
    std::random_device rd;
    std::mt19937 mt;

    std::uniform_real_distribution<f32> floatDist(0, 1);
    std::uniform_real_distribution<f64> doubleDist(0, 1);
    std::uniform_int_distribution<i32> intDist(0, 10);

    typedef std::uniform_real_distribution<f32>::param_type floatParamType;
    typedef std::uniform_real_distribution<f64>::param_type doubleParamType;
    typedef std::uniform_int_distribution<i32>::param_type intParamType;

    void internal_init()
    {
        mt = std::mt19937(rd());
    }

    f32 rangeF(f32 min, f32 max)
    {
        floatParamType t(min, max);
        floatDist.param(t);
        return floatDist(mt);
    }
    f64 rangeD(f64 min, f64 max)
    {
        doubleParamType t(min, max);
        doubleDist.param(t);
        return doubleDist(mt);
    }
    i32 rangeI(i32 min, i32 max)
    {
        intParamType t(min, max);
        intDist.param(t);
        return intDist(mt);
    }
}