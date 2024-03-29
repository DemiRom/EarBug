#ifndef FORMMAINWINDOW_H
#define FORMMAINWINDOW_H

#include <QWidget>

#include "FormMainWindow.h"
#include "PulseAudioController.h"
#include "FormSettingsWindow.h"
#include "uglobalhotkeys.h"

namespace dd::forms {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class FormMainWindow;
    }

    QT_END_NAMESPACE

    class FormMainWindow : public QWidget {
        Q_OBJECT

    public:
        explicit FormMainWindow(QWidget *parent = nullptr);

        ~FormMainWindow() override;

    private:
        Ui::FormMainWindow *ui;
        float masterVolumeLevel = 0;
        audio::PulseAudioController *pulseAudioController;
        FormSettingsWindow *settingsWindow = nullptr;

        void populateOutputDevices() const;


        bool masterVolumeMuted = false;

        [[nodiscard]] QIcon getSpeakerIconBasedOnVolume() const;

        void updateComputerMasterVolume();

    private slots:
        void showFromHotkey();

        void outputDeviceChanged(int index) const;

        void masterVolumeSliderChanged(float value);

        void masterVolumeMuteButtonPressed();

        void settingsButtonPressed() const;

        void applicationStateChanged(Qt::ApplicationState state);

    };
} // dd::forms

#endif //FORMMAINWINDOW_H
