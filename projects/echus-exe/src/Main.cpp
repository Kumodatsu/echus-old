#include <echus/SoundMachine.hpp>
#include <echus/Math.hpp>
#include <echus/Synthesis.hpp>
#include <echus/Instruments.hpp>
#include <nanogui/nanogui.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <unordered_map>

class App : public nanogui::Screen {
public:
    App()
        : nanogui::Screen(
            nanogui::Vector2i(500, 700), // size
            "Echus",                     // title
            true,                        // resizable
            false                        // fullscreen
        )
        , m_snd(0.01f)
        , m_instrument(echus::instruments::SawBass)
        , m_note(echus::NoteID::A, 0, 0)
    { 
        using namespace echus;
        
        nanogui::Window window(this, "Settings");

        draw_all();
        set_visible(true);
        perform_layout();
        window.center();

        SetUpInput();

        m_snd.StartAsync([this](float t) -> float {
            return this->m_instrument.Play(this->m_note, t);
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
};

int main(int, char**) {
    using namespace echus;
    
    nanogui::init();
    App app;
    
    nanogui::mainloop();

    nanogui::shutdown();
    return 0;
}
