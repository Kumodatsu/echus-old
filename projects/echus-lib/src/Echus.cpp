#include "echus/Echus.hpp"
#include "echus/SoundMachine.hpp"
#include "echus/Math.hpp"
#include <cmath>

namespace echus {

    void produce_sound() {
        SoundMachine snd(0.01f);
        static constexpr float
            lfo_f = 5.0f,
            lfo_a = 0.01f,
            f     = 2.0f * 440.0f;
        snd.Start([](float t) -> float {
            return std::sin(
                math::angular(f) * t
                + std::sin(math::angular(lfo_f) * t) * lfo_a * f
            );
        });
    }

}
