#pragma once
#include <cmath>
#include <limits>

namespace echus::math {

    inline constexpr float
        PI          = 3.141592653589f,
        C_12_ROOT_2 = 1.059463094359f,
        Infinity    = std::numeric_limits<float>::infinity();

    inline constexpr float angular(float f) {
        return 2.0f * PI * f;
    }

}
