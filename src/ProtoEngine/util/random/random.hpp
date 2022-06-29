#ifndef SRC_ENGINE_UTIL_RANDOM_RANDOM
#define SRC_ENGINE_UTIL_RANDOM_RANDOM

#include "util/types.h"

namespace Engine::Random
{
    void internal_init();

    i32 rangeI(i32 min, i32 max);
    f32 rangeF(f32 min, f32 max);
    f64 rangeD(f64 min, f64 max);
}

#endif // SRC_ENGINE_UTIL_RANDOM_RANDOM
