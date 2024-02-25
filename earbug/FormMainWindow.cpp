#include "FormMainWindow.h"
#include "ui_FormMainWindow.h"

#include <QAudioDevice>
#include <QMediaDevices>

#include "EarBug.h"
#include "PulseAudioController.h"
#include "FormSettingsWindow.h"
#include "WidgetAppVolumeControl.h"
#include "uglobalhotkeys.h"

namespace dd::forms {
    FormMainWindow::FormMainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::FormMainWindow) {
        ui->setupUi(this);

        //Create objects
        this->settingsWindow = new FormSettingsWindow();
        this->pulseAudioController = new audio::PulseAudioController("EarBug");
        this->pulseAudioController->collectSinks();

        for (const auto sinks = this->pulseAudioController->listSinks(); auto &sink: *sinks) {
            if (sink == nullptr) {
                qWarning() << "Sink is null!";
                continue;
            }

            this->ui->applicationVolumeControlVBoxLayout->addWidget(
                new widgets::WidgetAppVolumeControl(this->pulseAudioController, sink.get()));
        }

        //Set the main window flags.
        this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

        //Update the master volume slider
        // this->masterVolumeLevel = this->pulseAudioController->getMasterVolume();

        this->ui->masterVolumeSlider->setMaximum(PA_VOLUME_UI_MAX);
        this->ui->masterVolumeSlider->setMinimum(0);

        this->ui->masterVolumeSlider->setValue(PA_VOLUME_NORM); //TODO
        this->ui->applicationIconButton->setIcon(getSpeakerIconBasedOnVolume());

        //Populate output devices
        populateOutputDevices();

        //Set icons
        this->ui->applicationIconButton->setIcon(getSpeakerIconBasedOnVolume());

        const auto earBugSettingsManagerPtr = earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager();

        //Connect signals
        connect(this->ui->outputDeviceComboBox, &QComboBox::currentIndexChanged, this,
                &FormMainWindow::outputDeviceChanged);
        connect(this->ui->masterVolumeSlider, &QSlider::valueChanged, this, &FormMainWindow::masterVolumeSliderChanged);
        connect(this->ui->applicationIconButton, &QPushButton::pressed, this,
                &FormMainWindow::masterVolumeMuteButtonPressed);
        connect(this->ui->settingsButton, &QPushButton::pressed, this, &FormMainWindow::settingsButtonPressed);
        connect(qApp, &QGuiApplication::applicationStateChanged, this, &FormMainWindow::applicationStateChanged);
        connect(earBugSettingsManagerPtr, &earbug::settings::GlobalSettingsManager::showEarBugHotkeyPressed, this,
                &FormMainWindow::showFromHotkey);
    }

    FormMainWindow::~FormMainWindow() {
        delete settingsWindow;
        delete pulseAudioController;
        delete ui;
    }

    void FormMainWindow::populateOutputDevices() const {
        this->ui->outputDeviceComboBox->clear();

        const auto devices = QMediaDevices::audioOutputs();
        for (const QAudioDevice &device: devices) {
            this->ui->outputDeviceComboBox->addItem(device.description());
        }
    }

    QIcon FormMainWindow::getSpeakerIconBasedOnVolume() const {
        if (this->ui->masterVolumeSlider->value() <= (PA_VOLUME_UI_MAX / 3)) {
            return QIcon(":/sound_lowest.png");
        }

        if (this->ui->masterVolumeSlider->value() > (PA_VOLUME_UI_MAX / 3) && this->ui->masterVolumeSlider->value() <= (
                (PA_VOLUME_UI_MAX / 3) * 2)) {
            return QIcon(":/sound_lower.png");
        }

        return QIcon(":/sound.png");
    }

    void FormMainWindow::showFromHotkey() {
        if (auto *earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer(); earBugAppPtr->
            getSettingsManager()->getShowOnCurrentDisplay()) {
            for (const auto &screen: QApplication::screens()) {
                QPoint mousePos = QCursor::pos();
                QRect geometry = screen->geometry();
                if (mousePos.x() > geometry.left() && mousePos.x() < geometry.left() + screen->geometry().width() &&
                    mousePos.y() > geometry.top() && mousePos.y() < geometry.top() + screen->geometry().height()) {
                    earBugAppPtr->setDisplay(screen->name());
                }
            }
        }

        this->show();
        this->setFocus();
        this->activateWindow();
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void FormMainWindow::outputDeviceChanged(const int index) const {
        // NOLINT(*-convert-member-functions-to-static)
        qDebug("INDEX %i", index);
    }

    void FormMainWindow::masterVolumeSliderChanged(const float value) const {
        qDebug("Master Volume: %f", value);
        pulseAudioController->setMasterVolume(this->ui->masterVolumeSlider->value());
        this->ui->applicationIconButton->setIcon(getSpeakerIconBasedOnVolume());
        //TODO Save the master volume in the settings to re load last saved state
    }

    void FormMainWindow::masterVolumeMuteButtonPressed() {
        this->masterVolumeMuted = !this->masterVolumeMuted;

        // ReShaper bug - thinks this code is unreachable for some reason.
        // ReSharper disable once CppDFAUnreachableCode
        this->ui->applicationIconButton->setIcon(
            masterVolumeMuted ? QIcon(":/mute.png") : getSpeakerIconBasedOnVolume());

        this->pulseAudioController->setMuteMasterVolume(this->masterVolumeMuted);
    }

    void FormMainWindow::settingsButtonPressed() const {
        if (!settingsWindow->isHidden()) {
            settingsWindow->activateWindow();
            settingsWindow->setFocus();
        }
        settingsWindow->show();
    }

    void FormMainWindow::applicationStateChanged(const Qt::ApplicationState state) {
        if (!this->settingsWindow->isHidden())
            return;

        if (state == Qt::ApplicationInactive)
            this->hide();
    }
} // dd::forms
