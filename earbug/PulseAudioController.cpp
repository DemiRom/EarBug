#include "PulseAudioController.h"

#include <qlogging.h>

namespace dd::audio {
    PulseAudioController::PulseAudioController() {
        mainloop = pa_mainloop_new();
        mainloop_api = pa_mainloop_get_api(mainloop);
        context = pa_context_new(mainloop_api, "Volume Control Example");

        pa_context_set_state_callback(context, PulseAudioController::context_state_callback, NULL);
        pa_context_connect(context, NULL, PA_CONTEXT_NOFLAGS, NULL);

        while (pa_context_get_state(context) != PA_CONTEXT_READY) {
            pa_mainloop_iterate(mainloop, 1, NULL);
        }

        pa_operation *o = pa_context_get_sink_info_by_name(context, "@DEFAULT_SINK@",
                                                           PulseAudioController::sink_info_callback, NULL);

        while (pa_operation_get_state(o) == PA_OPERATION_RUNNING) {
            pa_mainloop_iterate(mainloop, 1, NULL);
        }

        pa_operation_unref(o);

        // get_master_volume();
    }

    PulseAudioController::~PulseAudioController() {
        // Disconnect and free resources
        pa_context_disconnect(context);
        pa_context_unref(context);
        pa_mainloop_free(mainloop);
    }

    float PulseAudioController::getMasterVolume() {
        return this->masterVolume; //TODO
    }

    void PulseAudioController::context_state_callback(pa_context *c, void *userdata) {
        pa_context_state_t state = pa_context_get_state(c);
        if (state == PA_CONTEXT_READY) {
            // The context is ready, you can perform operations on it here
            qDebug("Context Ready!");
        } else if (state == PA_CONTEXT_FAILED || state == PA_CONTEXT_TERMINATED) {
            fprintf(stderr, "Connection failed.\n");
        }
    }

    void PulseAudioController::volume_changed_callback(pa_context *c, int index, void *userdata) {
        qDebug("Volume changed\n");
    }

    void PulseAudioController::sink_info_callback(pa_context *c, const pa_sink_info *info, int eol, void *userdata) {
        if (eol > 0) {
            return;
        }

        // Print the volume of the default sink
        // printf("Master Volume: %s\n", pa_volume_snprint(info->volume.values, info->volume.channels));
        qDebug("Master Volume is: %i", info->volume.values[0]);
        qDebug("Application Name: %s\n", info->proplist ? pa_proplist_gets(info->proplist, "application.name") : "Unknown");
        // qDebug("Process ID: %u\n", info->proplist ? strtoul(pa_proplist_gets(info->proplist, "application.process.id"), NULL,  10) :  0);
    }

    void PulseAudioController::changeMasterVolume(const float level) const {
        const int volumeLevel = static_cast<int>(level * (PA_VOLUME_NORM / 100.0f));

        pa_operation *o;
        pa_cvolume volume;

        pa_cvolume_set(&volume, 2, volumeLevel);

        if (!(o = pa_context_set_sink_volume_by_name(context, "@DEFAULT_SINK@", &volume,
                                                     PulseAudioController::volume_changed_callback, NULL))) {
            qDebug("pa_context_set_sink_input_volume() failed");
            return;
        }

        pa_operation_unref(o);
    }
} // audio
