#ifndef SRC_ENGINE_UTIL_RANDOM_RANDOM
#define SRC_ENGINE_UTIL_RANDOM_RANDOM

namespace Engine::Random
{
    void internal_init();

    int rangeI(int min, int max);
    float rangeF(float min, float max);
    double rangeD(double min, double max);
}

#endif // SRC_ENGINE_UTIL_RANDOM_RANDOM
