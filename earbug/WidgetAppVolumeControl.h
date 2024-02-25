#ifndef WIDGETAPPVOLUMECONTROL_H
#define WIDGETAPPVOLUMECONTROL_H

#include <QWidget>

#include "PulseAudioController.h"

namespace dd::widgets {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class WidgetAppVolumeControl;
    }

    QT_END_NAMESPACE

    class WidgetAppVolumeControl final : public QWidget {
        Q_OBJECT

    public:
        explicit WidgetAppVolumeControl(audio::PulseAudioController* audioContoller, audio::Sink* sinkItem, QWidget *parent = nullptr);

        ~WidgetAppVolumeControl() override;

    private:
        Ui::WidgetAppVolumeControl *ui;
        audio::Sink* sinkItem;
        audio::PulseAudioController* audioController;

        QString clientName;

        [[nodiscard]] QIcon getSpeakerIconBasedOnVolume() const;

        bool muted;

    private slots:
        void volumeChanged(int value) const;

        void muteButtonPressed();

    };
} // dd::widgets

#endif //WIDGETAPPVOLUMECONTROL_H
