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
        , m_freq(0.0f)
        , m_instrument(new echus::instruments::Bell)
    { 
        using namespace echus;
        
        nanogui::Window window(this, "Settings");

        draw_all();
        set_visible(true);
        perform_layout();
        window.center();

        SetUpInput();

        m_snd.StartAsync([this](float t) -> float {
            return this->m_instrument->Play(this->m_freq, t);
        });
    }

    ~App() {
        delete m_instrument;
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
                    m_freq = echus::A2_FREQUENCY * std::pow(
                        echus::math::C_12_ROOT_2,
                        m_notes[kvp.first]
                    );
                }
            }
            if (any_key_pressed) {
                m_instrument->SetNoteOn(m_snd.GetTimeOffset());
            } else {
                m_instrument->SetNoteOff(m_snd.GetTimeOffset());
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

        m_notes[GLFW_KEY_Z]      = 0;
        m_notes[GLFW_KEY_S]      = 1;
        m_notes[GLFW_KEY_X]      = 2;
        m_notes[GLFW_KEY_C]      = 3;
        m_notes[GLFW_KEY_F]      = 4;
        m_notes[GLFW_KEY_V]      = 5;
        m_notes[GLFW_KEY_G]      = 6;
        m_notes[GLFW_KEY_B]      = 7;
        m_notes[GLFW_KEY_N]      = 8;
        m_notes[GLFW_KEY_J]      = 9;
        m_notes[GLFW_KEY_M]      = 10;
        m_notes[GLFW_KEY_K]      = 11;
        m_notes[GLFW_KEY_COMMA]  = 12;
        m_notes[GLFW_KEY_L]      = 13;
        m_notes[GLFW_KEY_PERIOD] = 14;
    }
private:
    echus::SoundMachine m_snd;
    float               m_freq;
    echus::Instrument*  m_instrument;

    std::unordered_map<int, bool> m_keys;
    std::unordered_map<int, int>  m_notes;
};

int main(int, char**) {
    using namespace echus;
    
    nanogui::init();
    App app;
    
    nanogui::mainloop();

    nanogui::shutdown();
    return 0;
}
