#pragma once
#include <soundio/soundio.h>
#include <functional>

namespace echus {

    class SoundMachine {
    public:
        SoundMachine(float master_volume = 0.1f);
        ~SoundMachine();

        void Start(std::function<float(float)> generate_sound);

        inline float GetMasterVolume() const { return m_master_volume; }
        inline void SetMasterVolume(float volume) { m_master_volume = volume; }
    private:
        SoundIo*          m_soundio;
        SoundIoDevice*    m_device;
        SoundIoOutStream* m_outstream;
        float             m_time_offset;

        std::function<float(float)> m_generate_sound;
        float                       m_master_volume;

        static void StreamCallback(SoundIoOutStream* outstream,
            int frame_count_min, int frame_count_max);
    };

}