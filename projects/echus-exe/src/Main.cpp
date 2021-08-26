#include <echus/SoundMachine.hpp>
#include <echus/Math.hpp>
#include <echus/Synthesis.hpp>
#include <echus/Instruments.hpp>
#include <glfw/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include "GUI.hpp"

class App : public nanogui::Screen {
public:
    App()
        : nanogui::Screen(
            nanogui::Vector2i(1440, 810), // size
            "Echus",                     // title
            true,                        // resizable
            false                        // fullscreen
        )
        , m_snd(0.01f)
        , m_instrument(echus::instruments::SawBass)
        , m_note(echus::NoteID::A, 0, 0)
    { 
        using namespace echus;
        
        SetUpUI();
        
        perform_layout();

        SetUpInput();

        m_snd.StartAsync([this](float t) -> float {
            const float a = this->m_instrument.Play(this->m_note, t);
            UpdateWaveGraph(a);
            return a;
        });
    }

    ~App() {
        
    }

    virtual bool keyboard_event(int key, int scancode, int action,
            int modifiers) override {
        if (Screen::keyboard_event(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            set_visible(false);
            return true;
        }
        if (action == GLFW_PRESS || action == GLFW_RELEASE) {
            bool any_key_pressed = false;
            for (auto& kvp : m_keys) {
                if (kvp.first == key) {
                    kvp.second = (action == GLFW_PRESS);
                }
                if (kvp.second) {
                    any_key_pressed = true;
                    m_note = m_notes[kvp.first];
                }
            }
            if (any_key_pressed) {
                m_note.StartTime = m_snd.GetTimeOffset();
            } else {
                m_note.EndTime = m_snd.GetTimeOffset();
            }
        }
        return false;
    }

    void SetUpUI() {
        using namespace nanogui;
        using namespace std::string_literals;

        auto* window = new Window(this, "Settings");
        window->set_position(Vector2i(15, 15));
        window->set_layout(new BoxLayout(Orientation::Horizontal));

        for (size_t i = 0; i < m_instrument.GetOscillators().size(); i++) {
            auto& osc = m_instrument.GetOscillator(i);
            
            auto* panel = new Widget(window);
            panel->set_layout(new GroupLayout);

            new Label(panel, "Oscillator "s + std::to_string(i));
            new Label(panel, "Wave Shape");
            create_combo_box<echus::WaveShape>(
                *panel,
                {
                    "Sine",
                    "Square",
                    "Triangle",
                    "Sawtooth",
                    "Sawsmooth",
                    "Psuedo Random Noise",
                    "Off"
                },
                osc.Shape,
                [&osc](echus::WaveShape shape) {
                    osc.Shape = shape;
                }
            );
            create_slider(*panel, "Amplitude", 0.0f, 1.0f, osc.Amplitude,
                [&osc](float v) { osc.Amplitude = v; });
            create_slider(*panel, "Scale", 0.0f, 16.0f, osc.Scale,
                [&osc](float v) { osc.Scale = v; });
            create_slider(*panel, "LFO Frequency", "Hz", 0.0f, 300.0f,
                osc.LFOFreq, [&osc](float v) { osc.LFOFreq = v; });
            create_slider(*panel, "LFO Amplitude", 0.0f, 1.0f, osc.LFOAmp,
                [&osc](float v) { osc.LFOAmp = v; });
        }
        
        auto* panel = new Widget(window);
        panel->set_layout(new GroupLayout);
        new Label(panel, "Envelope");
        echus::Envelope& env = m_instrument.GetEnvelope();
        create_slider(*panel, "Attack", "s", 0.0f, 10.0f, env.Attack,
            [&env](float v) { env.Attack = v; });
        create_slider(*panel, "Decay", "s", 0.0f, 10.0f, env.Decay,
            [&env](float v) { env.Decay = v; });
        create_slider(*panel, "Release", "s", 0.0f, 10.0f, env.Release,
            [&env](float v) { env.Release = v; });
        create_slider(*panel, "Peak Amplitude", 0.0f, 1.0f, env.Peak,
            [&env](float v) { env.Peak = v; });
        create_slider(*panel, "Sustain Amplitude", 0.0f, 1.0f, env.Sustain,
            [&env](float v) { env.Sustain = v; });

        m_wave_graph = new Graph(window, "Wave");
        m_wave_graph->set_fill_color(Color(0.0f, 0.0f));
        m_wave_graph->set_stroke_color(Color(0.1f, 0.9f, 0.3f, 1.0f));
        std::vector<float>& values = m_wave_graph->values();
        values.resize(448);
        for (int i = 0; i < values.size(); i++) {
            values[i] = 0.5f;
        }
    }

    void UpdateWaveGraph(float a) {
        std::vector<float>& values = m_wave_graph->values();
        values.erase(values.begin());
        values.push_back(0.5f + 0.5f * a);
    }

    void SetUpInput() {
        m_keys[GLFW_KEY_Z]      =
        m_keys[GLFW_KEY_S]      =
        m_keys[GLFW_KEY_X]      =
        m_keys[GLFW_KEY_C]      =
        m_keys[GLFW_KEY_F]      =
        m_keys[GLFW_KEY_V]      =
        m_keys[GLFW_KEY_G]      =
        m_keys[GLFW_KEY_B]      =
        m_keys[GLFW_KEY_N]      =
        m_keys[GLFW_KEY_J]      =
        m_keys[GLFW_KEY_M]      =
        m_keys[GLFW_KEY_K]      =
        m_keys[GLFW_KEY_COMMA]  =
        m_keys[GLFW_KEY_L]      =
        m_keys[GLFW_KEY_PERIOD] =
            false;

        m_notes.insert({
            { GLFW_KEY_Z,      echus::Note(echus::NoteID::A,      0, 0) },
            { GLFW_KEY_S,      echus::Note(echus::NoteID::ASharp, 0, 0) },
            { GLFW_KEY_X,      echus::Note(echus::NoteID::B,      0, 0) },
            { GLFW_KEY_C,      echus::Note(echus::NoteID::C,      0, 0) },
            { GLFW_KEY_F,      echus::Note(echus::NoteID::CSharp, 0, 0) },
            { GLFW_KEY_V,      echus::Note(echus::NoteID::D,      0, 0) },
            { GLFW_KEY_G,      echus::Note(echus::NoteID::DSharp, 0, 0) },
            { GLFW_KEY_B,      echus::Note(echus::NoteID::E,      0, 0) },
            { GLFW_KEY_N,      echus::Note(echus::NoteID::F,      0, 0) },
            { GLFW_KEY_J,      echus::Note(echus::NoteID::FSharp, 0, 0) },
            { GLFW_KEY_M,      echus::Note(echus::NoteID::G,      0, 0) },
            { GLFW_KEY_K,      echus::Note(echus::NoteID::GSharp, 0, 0) },
            { GLFW_KEY_COMMA,  echus::Note(echus::NoteID::A,      1, 0) },
            { GLFW_KEY_L,      echus::Note(echus::NoteID::ASharp, 1, 0) },
            { GLFW_KEY_PERIOD, echus::Note(echus::NoteID::B,      1, 0) }
        });
    }
private:
    echus::SoundMachine m_snd;
    echus::Instrument   m_instrument;
    echus::Note         m_note;

    std::unordered_map<int, bool>         m_keys;
    std::unordered_map<int, echus::Note>  m_notes;

    nanogui::Graph* m_wave_graph;
};

int main(int, char**) {
    using namespace echus;
    
    nanogui::init();
    App app;
    app.set_visible(true);
    app.draw_all();
    
    nanogui::mainloop(16.0f);

    nanogui::shutdown();
    return 0;
}
