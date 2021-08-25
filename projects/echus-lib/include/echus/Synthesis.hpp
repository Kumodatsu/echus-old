#pragma once
#include <cstdint>
#include "echus/Math.hpp"
#include "echus/Music.hpp"

namespace echus {

    enum class WaveShape : uint8_t {
        Sine,
        Square,
        Triangle,
        Sawtooth,
        Sawsmooth,
        PsuedoRandomNoise
    };

    struct Oscillator {
        WaveShape Shape;

        Oscillator(WaveShape shape) : Shape(shape) { }

        inline float Oscillate(float t, float freq, float lfo_freq = 0.0f,
                float lfo_amp = 0.0f) const {
            const float f
                = math::angular(freq) * t
                + std::sin(math::angular(lfo_freq) * t) * lfo_amp * freq;
            switch (Shape) {
            case WaveShape::Sine:
                return std::sin(f);
            case WaveShape::Square:
                return std::sin(f) > 0.0f ? 1.0f : -1.0f;
            case WaveShape::Triangle:
                return std::asin(std::sin(f)) * 2.0f / math::PI;
            case WaveShape::Sawtooth:
                return (2.0f / math::PI) * (
                    freq * math::PI * std::fmod(t, 1.0f / freq)
                        - (math::PI / 2.0f)
                );
            case WaveShape::Sawsmooth: {
                float output = 0.0f;
                for (float n = 1.0f; n < 100.0f; n += 1.0f)
                    output += std::sin(n * f) / n;
                return output;
            }
            case WaveShape::PsuedoRandomNoise:
                return std::sin(
                    2.0f * (float(rand()) / float(RAND_MAX)) - 1.0f
                );
            default:
                return 0.0f;
            }
        }
    };

    struct Envelope {
        float Attack,
              Decay,
              Release,
              Peak,
              Sustain;

        Envelope(
            float attack           = 0.0f,
            float decay            = 0.0f,
            float release          = 0.0f,
            float peak             = 1.0f,
            float sustain          = 1.0f
        )
            : Attack(attack)
            , Decay(decay)
            , Release(release)
            , Peak(peak)
            , Sustain(sustain)
        { }

        inline float AmplitudeAt(float t, const Note& note) const {
            static constexpr float epsilon = 1.0e-4f;
            float amplitude = 0.0f;
            if (note.IsOnAt(t)) {
                const float life_time = t - note.StartTime;
                if (life_time <= Attack) {
                    // Attack phase
                    const float portion = life_time / Attack;
                    amplitude = portion * Peak;
                } else if (life_time <= Attack + Decay) {
                    // Decay phase
                    const float portion = (life_time - Attack) / Decay;
                    amplitude = Peak + portion * (Sustain - Peak);
                } else {
                    // Sustain phase
                    amplitude = Sustain;
                }
            } else if (t > note.EndTime) {
                // Release phase
                const float portion = (t - note.EndTime) / Release;
                amplitude = (1.0f - portion) * Sustain;
            }
            if (amplitude < epsilon)
                amplitude = 0.0f;
            return amplitude;
        }
    };

    class Instrument {
    public:
        Instrument(float volume = 1.0f) : m_volume(volume) { }

        inline float Play(const Note& note, float t) const {
            return m_volume * Sound(note, t);
        }

        inline float GetVolume() const { return m_volume; }
        inline void SetVolume(float volume) { m_volume = volume; }
    protected:
        virtual float Sound(const Note& note, float t) const = 0;
    private:
        float m_volume;
    };

}
