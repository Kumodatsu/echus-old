#pragma once
#include <soundio/soundio.h>
#include <functional>

namespace echus {

    class SoundMachine {
    public:
        SoundMachine(float master_volume = 0.1f, float target_latency = 0.05f);
        ~SoundMachine();

        void StartAsync(std::function<float(float)> generate_sound);
        void WaitForEvents();

        inline float GetTimeOffset() const { return m_time_offset; }
        inline float GetMasterVolume() const { return m_master_volume; }
        inline void SetMasterVolume(float volume) { m_master_volume = volume; }
    private:
        SoundIo*          m_soundio;
        SoundIoDevice*    m_device;
        SoundIoOutStream* m_outstream;
        float             m_time_offset;

        std::function<float(float)> m_generate_sound;
        float                       m_master_volume;
        float                       m_target_latency;

        static void StreamCallback(SoundIoOutStream* outstream,
            int frame_count_min, int frame_count_max);
    };

}