#pragma once
#include <cstdint>
#include "echus/Math.hpp"

namespace echus {

    inline constexpr float A2_FREQUENCY = 110.0f;

    enum class NoteID : uint8_t {
        A      = 0,
        ASharp = 1,
        BFlat  = ASharp,
        B      = 2,
        C      = 3,
        CSharp = 4,
        DFlat  = CSharp,
        D      = 5,
        DSharp = 6,
        EFlat  = DSharp,
        E      = 7,
        F      = 8,
        FSharp = 9,
        GFlat  = FSharp,
        G      = 10,
        GSharp = 11,
        AFlat  = GSharp
    };

    struct Note {
        NoteID  ID;
        int32_t Octave,
                Channel;
        float   StartTime,
                EndTime;
        
        Note() = default;

        Note(
            NoteID  id,
            int32_t octave,
            int32_t channel    = 0,
            float   start_time = math::Infinity,
            float   end_time   = math::Infinity
        )
            : ID(id)
            , Octave(octave)
            , Channel(channel)
            , StartTime(start_time)
            , EndTime(end_time)
        { }

        inline int32_t GetStep() const {
            return static_cast<int32_t>(ID) + 12 * Octave;
        }

        inline float GetFrequency() const {
            return A2_FREQUENCY * std::pow(math::C_12_ROOT_2, GetStep());
        }

        inline bool IsOnAt(float t) const {
            return StartTime <= t && t < EndTime;
        }
    };

}
