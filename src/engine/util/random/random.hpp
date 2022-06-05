#ifndef SRC_ENGINE_UTIL_RANDOM_RANDOM
#define SRC_ENGINE_UTIL_RANDOM_RANDOM

namespace Engine::Random
{
    void internal_engine_init();

    float rangeI(float min, float max);
    double rangeF(double min, double max);
    int rangeD(int min, int max);
}

#endif // SRC_ENGINE_UTIL_RANDOM_RANDOM
