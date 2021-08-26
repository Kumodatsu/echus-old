#pragma once
#include <nanogui/nanogui.h>
#include <string>
#include <functional>

inline void create_slider(
    nanogui::Widget&           parent,
    const std::string&         label,
    const std::string&         unit_name,
    float                      min,
    float                      max,
    std::function<void(float)> callback
) {
    using namespace nanogui;
    
    new Label(&parent, label);
    auto* display = new Label(&parent, std::to_string(0.0f) + unit_name);
    (new Slider(&parent))->set_callback(
        [unit_name, min, max, callback, display] (float value) {
            const float scaled_value = min + value * (max - min);
            display->set_caption(std::to_string(scaled_value) + unit_name);
            callback(scaled_value);
        }
    );
}

inline void create_slider(
    nanogui::Widget&           parent,
    const std::string&         label,
    float                      min,
    float                      max,
    std::function<void(float)> callback
) {
    create_slider(parent, label, "", min, max, callback);
}
