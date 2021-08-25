#pragma once
#include "echus/Synthesis.hpp"

namespace echus::instruments {

    class Harmonica : public Instrument {
    public:
        Harmonica()
            : Instrument(1.0f)
            , m_osc0(WaveShape::Square)
            , m_osc1(WaveShape::Square)
            , m_osc2(WaveShape::Square)
            , m_osc3(WaveShape::PsuedoRandomNoise)
            , m_envelope(0.1f, 0.01f, 0.2f, 1.0f, 0.8f)
        { }

        virtual void SetNoteOn(float t) override {
            m_envelope.SetNoteOn(t);
        }

        virtual void SetNoteOff(float t) override {
            m_envelope.SetNoteOff(t);
        }
    protected:
        virtual float Sound(float freq, float t) const override {
            return m_envelope(t) * (
                + 1.00f * m_osc0.Oscillate(t, 1.0f * freq, 5.0f, 0.001f)
                + 0.50f * m_osc1.Oscillate(t, 1.5f * freq)
                + 0.25f * m_osc2.Oscillate(t, 2.0f * freq)
                + 0.05f * m_osc3.Oscillate(t, 0.0f)
            );
        }
    private:
        Oscillator m_osc0, m_osc1, m_osc2, m_osc3;
        Envelope   m_envelope;
    };

    class Bell : public Instrument {
    public:
        Bell()
            : Instrument(1.0f)
            , m_osc0(WaveShape::Sine)
            , m_osc1(WaveShape::Sine)
            , m_osc2(WaveShape::Sine)
            , m_envelope(0.01f, 1.0f, 1.0f, 1.0f, 0.0f)
        { }

        virtual void SetNoteOn(float t) override {
            m_envelope.SetNoteOn(t);
        }

        virtual void SetNoteOff(float t) override {
            m_envelope.SetNoteOff(t);
        }
    protected:
        virtual float Sound(float freq, float t) const override {
            return m_envelope(t) * (
                + 1.00f * m_osc0.Oscillate(t, 2.0f * freq, 5.0f, 0.001f)
                + 0.50f * m_osc1.Oscillate(t, 3.0f * freq)
                + 0.25f * m_osc2.Oscillate(t, 4.0f * freq)
            );
        }
    private:
        Oscillator m_osc0, m_osc1, m_osc2;
        Envelope   m_envelope;
    };

}
