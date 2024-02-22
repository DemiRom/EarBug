#ifndef PULSEAUDIO_H
#define PULSEAUDIO_H

#include <pulse/pulseaudio.h>

namespace dd::audio {
    class PulseAudioController {
    public:
        PulseAudioController();
        ~PulseAudioController();
        void changeMasterVolume(float level) const;
        float getMasterVolume() const;

    private:
        pa_mainloop *mainloop;
        pa_mainloop_api *mainloop_api;
        pa_context *context;

        static void context_state_callback(pa_context *c, void *userdata);
        static void volume_changed_callback(pa_context *c, int index, void *userdata);
        static void sink_info_callback(pa_context *c, const pa_sink_info *info, int eol, void *userdata);

        void get_master_volume() const;
    };
} // audio
// dd

#endif //PULSEAUDIO_H
