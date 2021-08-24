#include "echus/SoundMachine.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>
#include <algorithm>
#include "echus/Math.hpp"

namespace echus {

    SoundMachine::SoundMachine(float master_volume, float target_latency)
        : m_soundio(nullptr)
        , m_device(nullptr)
        , m_outstream(nullptr)
        , m_time_offset(0.0f)
        , m_generate_sound(nullptr)
        , m_master_volume(master_volume)
        , m_target_latency(target_latency)
    {
        using namespace std::literals::string_literals;

        int error;
        if (!(m_soundio = soundio_create()))
            throw std::runtime_error("Out of memory.");
        if ((error = soundio_connect(m_soundio)))
            throw std::runtime_error(
                "Error connecting: "s + soundio_strerror(error)
            );
        soundio_flush_events(m_soundio);
        const int device_index = soundio_default_output_device_index(m_soundio);
        if (device_index < 0)
            throw std::runtime_error("No output device found.");
        if (!(m_device = soundio_get_output_device(m_soundio, device_index)))
            throw std::runtime_error("Out of memory.");
        if (!(m_outstream = soundio_outstream_create(m_device)))
            throw std::runtime_error("Out of memory.");
        m_outstream->format         = SoundIoFormatFloat32NE;
        m_outstream->write_callback = nullptr;
        m_outstream->userdata       = this;

        // TODO: Remove temporary print
        std::cout << "Output device: " << m_device->name << std::endl;
    }

    SoundMachine::~SoundMachine() {
        soundio_outstream_destroy(m_outstream);
        soundio_device_unref(m_device);
        soundio_destroy(m_soundio);    
    }

    void SoundMachine::StartAsync(std::function<float(float)> generate_sound) {
        using namespace std::literals::string_literals;

        m_generate_sound            = generate_sound;
        m_outstream->write_callback = StreamCallback;

        int error;

        if ((error = soundio_outstream_open(m_outstream)))
            throw std::runtime_error("Unable to open device.");
        if (m_outstream->layout_error)
            throw std::runtime_error("Unable to set channel layout: "s
                + soundio_strerror(m_outstream->layout_error));
        if ((error = soundio_outstream_start(m_outstream)))
            throw std::runtime_error("Unable to start device.");
    }

    void SoundMachine::WaitForEvents() {
        soundio_wait_events(m_soundio);
    }
    
    void SoundMachine::StreamCallback(
        SoundIoOutStream* outstream,
        int               frame_count_min,
        int               frame_count_max
    ) {
        auto* snd_ = reinterpret_cast<SoundMachine*>(outstream->userdata);
        SoundMachine& snd = *snd_;

        const SoundIoChannelLayout& layout = outstream->layout;
        const float
            sample_rate       = static_cast<float>(outstream->sample_rate),
            seconds_per_frame = 1.0f / sample_rate;

        SoundIoChannelArea* areas;
        int frames_left = std::clamp(
            static_cast<int>(snd.m_target_latency * sample_rate),
            frame_count_min,
            frame_count_max
        );

        int error;

        while (frames_left > 0) {
            int frame_count = frames_left;
            
            if ((error = soundio_outstream_begin_write(outstream, &areas,
                    &frame_count))) {
                throw std::runtime_error(soundio_strerror(error));
            }
            if (!frame_count)
                break;

            for (int frame = 0; frame < frame_count; frame++) {
                const float
                    t = snd.m_time_offset + frame * seconds_per_frame,
                    a = snd.m_generate_sound(t);
                for (int channel = 0; channel < layout.channel_count;
                        channel++) {
                    float* sample = reinterpret_cast<float*>(
                        areas[channel].ptr + areas[channel].step * frame
                    );
                    *sample = snd.GetMasterVolume() * a;
                }
            }
            
            snd.m_time_offset = std::fmod(
                snd.m_time_offset + seconds_per_frame * frame_count,
                1.0f
            );

            if ((error = soundio_outstream_end_write(outstream)))
                throw std::runtime_error(soundio_strerror(error));

            frames_left -= frame_count;
        }
    }
    
}
