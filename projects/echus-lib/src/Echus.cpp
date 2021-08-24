#include "echus/Echus.hpp"
#include <soundio/soundio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdexcept>

namespace echus {

    static const float PI = 3.1415926535f;
    static float seconds_offset = 0.0f;
    static void write_callback(struct SoundIoOutStream *outstream,
            int frame_count_min, int frame_count_max)
    {
        const struct SoundIoChannelLayout *layout = &outstream->layout;
        float float_sample_rate = outstream->sample_rate;
        float seconds_per_frame = 1.0f / float_sample_rate;
        struct SoundIoChannelArea *areas;
        int frames_left = frame_count_max;
        int err;

        while (frames_left > 0) {
            int frame_count = frames_left;

            if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
                fprintf(stderr, "%s\n", soundio_strerror(err));
                exit(1);
            }

            if (!frame_count)
                break;

            float pitch = 440.0f;
            float radians_per_second = pitch * 2.0f * PI;
            for (int frame = 0; frame < frame_count; frame += 1) {
                constexpr float volume = 0.1;
                const float t
                    = radians_per_second
                    * (seconds_offset + frame * seconds_per_frame);
                float sample = sinf(t) > 0.0 ? 1.0 : -1.0; // 2.0 * (float(rand()) / float(RAND_MAX)) - 1.0; // sinf(t);
                for (int channel = 0; channel < layout->channel_count; channel += 1) {
                    float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                    *ptr = volume * sample;
                }
            }
            seconds_offset = fmodf(seconds_offset +
                seconds_per_frame * frame_count, 1.0f);

            if ((err = soundio_outstream_end_write(outstream))) {
                fprintf(stderr, "%s\n", soundio_strerror(err));
                exit(1);
            }

            frames_left -= frame_count;
        }
    }

    void produce_sound() {
        int err;
        struct SoundIo *soundio = soundio_create();
        if (!soundio) {
            fprintf(stderr, "out of memory\n");
            throw std::runtime_error("out of memory");
        }

        if ((err = soundio_connect(soundio))) {
            fprintf(stderr, "error connecting: %s", soundio_strerror(err));
            throw std::runtime_error("error connecting");
        }

        soundio_flush_events(soundio);

        int default_out_device_index = soundio_default_output_device_index(soundio);
        if (default_out_device_index < 0) {
            fprintf(stderr, "no output device found");
            throw std::runtime_error("no output device found");
        }

        struct SoundIoDevice *device = soundio_get_output_device(soundio, default_out_device_index);
        if (!device) {
            fprintf(stderr, "out of memory");
            throw std::runtime_error("out of memory");
        }

        fprintf(stderr, "Output device: %s\n", device->name);

        struct SoundIoOutStream *outstream = soundio_outstream_create(device);
        outstream->format = SoundIoFormatFloat32NE;
        outstream->write_callback = write_callback;

        if ((err = soundio_outstream_open(outstream))) {
            fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
            throw std::runtime_error("unable to open device");
        }

        if (outstream->layout_error)
            fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

        if ((err = soundio_outstream_start(outstream))) {
            fprintf(stderr, "unable to start device: %s", soundio_strerror(err));
            throw std::runtime_error("unable to start device");
        }

        for (;;)
            soundio_wait_events(soundio);

        soundio_outstream_destroy(outstream);
        soundio_device_unref(device);
        soundio_destroy(soundio);    
    }

}
