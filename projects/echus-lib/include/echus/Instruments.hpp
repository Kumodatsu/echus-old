#pragma once
#include "echus/Synthesis.hpp"

namespace echus::instruments {

    const Instrument
        Harmonica(
            {
                Oscillator(WaveShape::Square, 1.00f, 1.0f, 5.0f, 0.001f),
                Oscillator(WaveShape::Square, 0.50f, 1.5f),
                Oscillator(WaveShape::Square, 0.25f, 2.0f),
                Oscillator(WaveShape::PsuedoRandomNoise, 0.05f, 0.0f)
            },
            Envelope(0.1f, 0.01f, 0.2f, 1.0f, 0.8f)
        ),
        Bell(
            {
                Oscillator(WaveShape::Sine, 1.00f, 2.0f, 5.0f, 0.001f),
                Oscillator(WaveShape::Sine, 0.50f, 3.0f),
                Oscillator(WaveShape::Sine, 0.25f, 4.0f)
            },
            Envelope(0.01f, 1.0f, 1.0f, 1.0f, 0.0f)
        ),
        Square(
            {
                Oscillator(WaveShape::Square)
            },
            Envelope()
        ),
        SawBass(
            {
                Oscillator(WaveShape::Sawtooth, 1.0f, 1.0f),
                Oscillator(WaveShape::Sine,     1.0f, 2.0f)
            },
            Envelope()
        );

}
