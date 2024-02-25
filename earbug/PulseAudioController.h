#ifndef PULSEAUDIO_H
#define PULSEAUDIO_H

#include <pulse/pulseaudio.h>

#include <pulse/thread-mainloop.h>
#include <pulse/context.h>
#include <pulse/introspect.h>
#include <pulse/stream.h>
#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <memory>

namespace dd::audio {
    struct Sink {
        uint32_t idx;
        uint32_t client_idx;
        std::string name;
        std::string sink_name;
        std::string client_name;
        bool muted;
        pa_cvolume volume;
        bool sound;
    };

    struct Detector {
        pa_stream *stream;
        int8_t sound;
    };

    typedef std::shared_ptr<Sink> PSink;
    typedef std::shared_ptr<pa_sink_input_info> PSinkInfo;
    typedef std::shared_ptr<Detector> PDetector;

    enum ChangeType {
        NONE, FULL, REDRAW
    };

    class PulseAudioController final {
    public:
        explicit PulseAudioController(std::string app_name = "earbug");

        ~PulseAudioController();

        friend void sink_input_info_list_cb(pa_context *c,
                                            const pa_sink_input_info *i, int eol, void *userdata);

        void collectSinks();

        void addSink(PSinkInfo sink);

        void refreshSink(uint32_t idx);

        unsigned int getSinksCount();

        std::shared_ptr<std::vector<PSink> > listSinks();

        PSink changeVolume(uint32_t index, int change, bool inc);

        PSink setVolume(uint32_t index, int vol);

        int getVolume(uint32_t index);

        void setMasterVolume(int vol) const;

        void setMuteMasterVolume(bool mute) const;

        PSink toggleMute(uint32_t index);

        void setClientName(const char *name);

        void setExternalChange(ChangeType type);

        ChangeType getExternalChange();

        void setSoundState(uint32_t idx, bool sound);

    private:
        pa_sample_spec sampleSpec{PA_SAMPLE_U8, 8000, 1};
        pa_threaded_mainloop *mainloop;
        pa_context *context;
        std::map<uint32_t, PSink> sinks;
        std::map<uint32_t, PDetector> detectorStreams;

        PSink wrap_sink(PSinkInfo sink);

        std::string client_name;
        std::string app_name;

        friend void complete(void *userdata);

        void wait(pa_operation *o, bool debug = false);

        void set_vol(uint32_t index, pa_cvolume *pvol);

        volatile ChangeType external_change = NONE;
    };
} // audio
// dd

#endif //PULSEAUDIO_H
