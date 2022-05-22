#ifndef SRC_ENGINE_UTIL_RANDOM_RANDOM
#define SRC_ENGINE_UTIL_RANDOM_RANDOM

#include <random>

namespace Engine::Random
{
    void engine_init();

    float float_(float min, float max);
    double double_(double min, double max);
    int int_(int min, int max);
}

#endif // SRC_ENGINE_UTIL_RANDOM_RANDOM
