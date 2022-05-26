#ifndef SRC_ENGINE_UTIL_RANDOM_RANDOM
#define SRC_ENGINE_UTIL_RANDOM_RANDOM

namespace Engine::Random
{
    void engine_init();

    float range(float min, float max);
    double range(double min, double max);
    int range(int min, int max);
}

#endif // SRC_ENGINE_UTIL_RANDOM_RANDOM
