#pragma once
#include <nanogui/nanogui.h>
#include <string>
#include <functional>
#include <algorithm>
#include <type_traits>

inline nanogui::Slider& create_slider(
    nanogui::Widget&           parent,
    const std::string&         label,
    const std::string&         unit_name,
    float                      min,
    float                      max,
    float                      initial_value,
    std::function<void(float)> on_value_changed
) {
    using namespace nanogui;
    
    new Label(&parent, label);
    auto* display = new Label(&parent, std::to_string(0.0f) + unit_name);
    auto* slider = new Slider(&parent);
    const auto callback =
        [unit_name, min, max, on_value_changed, display] (float value) {
            // const float scaled_value = min + value * (max - min);
            display->set_caption(std::to_string(value) + unit_name);
            on_value_changed(value);
        };
    const float value = std::clamp(initial_value, min, max);
    slider->set_range({min, max});
    slider->set_callback(callback);
    slider->set_value(value);
    callback(value);
    return *slider;
}

inline nanogui::Slider& create_slider(
    nanogui::Widget&           parent,
    const std::string&         label,
    float                      min,
    float                      max,
    float                      initial_value,
    std::function<void(float)> on_value_changed
) {
    return create_slider(parent, label, "", min, max, initial_value,
        on_value_changed);
}

template <typename EnumT>
inline nanogui::ComboBox& create_combo_box(
    nanogui::Widget&                parent,
    const std::vector<std::string>& items,
    EnumT                           initial_value,
    std::function<void(EnumT)>      on_value_changed
) {
    static_assert(std::is_enum_v<EnumT>, "EnumT must be an enum type.");
    using namespace nanogui;

    auto* combo_box = new ComboBox(&parent, items);
    const auto callback = [on_value_changed] (int value) {
        on_value_changed(static_cast<EnumT>(value));
    };
    const int value = static_cast<int>(initial_value);
    combo_box->set_callback(callback);
    combo_box->set_selected_index(value);
    callback(value);
    return *combo_box;
}
