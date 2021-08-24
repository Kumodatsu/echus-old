#pragma once
#include <cmath>

namespace echus::math {

    inline constexpr float
        PI          = 3.141592653589f,
        C_12_ROOT_2 = 1.059463094359295264;

    inline constexpr float angular(float f) {
        return 2.0f * PI * f;
    }

}
