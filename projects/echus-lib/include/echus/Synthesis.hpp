#pragma once
#include <cstdint>
#include <vector>
#include "echus/Math.hpp"
#include "echus/Music.hpp"

namespace echus {

    enum class WaveShape : uint8_t {
        Sine,
        Square,
        Triangle,
        Sawtooth,
        Sawsmooth,
        PsuedoRandomNoise,
        Line
    };

    struct Oscillator {
        WaveShape Shape;
        float     Amplitude,
                  Scale,
                  LFOFreq,
                  LFOAmp;

        Oscillator(
            WaveShape shape,
            float amplitude = 1.0f,
            float scale     = 1.0f,
            float lfo_freq  = 0.0f,
            float lfo_amp   = 0.0f
        )
            : Shape(shape)
            , Amplitude(amplitude)
            , Scale(scale)
            , LFOFreq(lfo_freq)
            , LFOAmp(lfo_amp)
        { }

        inline float Oscillate(float t, float freq) const {
            const float sf = Scale * freq;
            const float f
                = math::angular(sf) * t
                + std::sin(math::angular(LFOFreq) * t) * LFOAmp * sf;
            
            float output = 0.0f;
            switch (Shape) {
            case WaveShape::Sine:
                output = std::sin(f);
                break;
            case WaveShape::Square:
                output = std::sin(f) > 0.0f ? 1.0f : -1.0f;
                break;
            case WaveShape::Triangle:
                output = std::asin(std::sin(f)) * 2.0f / math::PI;
                break;
            case WaveShape::Sawtooth:
                output = (2.0f / math::PI) * (
                    sf * math::PI * std::fmod(t, 1.0f / sf)
                        - (math::PI / 2.0f)
                );
                break;
            case WaveShape::Sawsmooth: {
                for (float n = 1.0f; n < 100.0f; n += 1.0f)
                    output += std::sin(n * f) / n;
                break;
            }
            case WaveShape::PsuedoRandomNoise:
                output = std::sin(
                    2.0f * (float(rand()) / float(RAND_MAX)) - 1.0f
                );
                break;
            case WaveShape::Line:
            default:
                output = 0.0f;
                break;
            }

            return Amplitude * output;
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
        Instrument(
            const std::vector<Oscillator>& oscillators,
            const Envelope&                envelope,
            float                          volume = 1.0f
        )
            : m_oscillators(oscillators)
            , m_envelope(envelope)
            , m_volume(volume)
        { }

        inline float Play(const Note& note, float t) const {
            const float freq = note.GetFrequency();
            float amplitude = 0.0f;
            for (const auto& osc : m_oscillators)
                amplitude += osc.Oscillate(t, freq);
            return m_volume * m_envelope.AmplitudeAt(t, note) * amplitude;
        }

        inline float GetVolume() const { return m_volume; }
        inline void SetVolume(float volume) { m_volume = volume; }

        inline const std::vector<Oscillator>& GetOscillators() const {
            return m_oscillators;
        }

        inline std::vector<Oscillator>& GetOscillators() {
            return m_oscillators;
        }

        inline const Oscillator& GetOscillator(size_t i) const {
            return m_oscillators[i];
        }

        inline Oscillator& GetOscillator(size_t i) {
            return m_oscillators[i];
        }

        inline const Envelope& GetEnvelope() const {
            return m_envelope;
        }

        inline Envelope& GetEnvelope() {
            return m_envelope;
        }
    private:
        std::vector<Oscillator> m_oscillators;
        Envelope                m_envelope;
        float                   m_volume;
    };

}
