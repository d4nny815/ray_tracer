#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <cstdlib>

using std::make_shared;
using std::shared_ptr;

const float MY_INFINITY = std::numeric_limits<float>::infinity();
const float PI = 3.1415926535897932385;

inline float degrees_to_radians(float degrees) {
    return degrees * PI / 180.0;
}

// inline float rand_float() {
//     return std::rand() / (RAND_MAX + 1.0);
// }

// inline float rand_float(float min, float max) {
//     return min + (max - min) * rand_float();
// }

class RandLCG {
public:
    uint32_t state;

    RandLCG(uint32_t seed = 1) : state(seed) {}

    inline float next_float() {
        state = 1664525u * state + 1013904223u;
        return (state & 0x00FFFFFF) / static_cast<float>(0x01000000); // 24-bit precision
    }

    inline float next_float(float min, float max) {
        return min + (max - min) * next_float();
    }
};

#endif /* util.h */
