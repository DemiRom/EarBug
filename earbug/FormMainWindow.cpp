#include "FormMainWindow.h"
#include "ui_FormMainWindow.h"

#include <QAudioDevice>
#include <QMediaDevices>

#include "PulseAudioController.h"
#include "FormSettingsWindow.h"

namespace dd::forms {
    FormMainWindow::FormMainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::FormMainWindow) {
        ui->setupUi(this);

        this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

        pulseAudioController = new audio::PulseAudioController();

        pulseAudioController->getMasterVolume(); //TODO

        populateOutputDevices();

        this->ui->applicationIconButton->setIcon(getSpeakerIconBasedOnVolume());

        connect(this->ui->outputDeviceComboBox, &QComboBox::currentIndexChanged, this,
                &FormMainWindow::outputDeviceChanged);
        connect(this->ui->masterVolumeSlider, &QSlider::valueChanged, this, &FormMainWindow::masterVolumeSliderChanged);
        connect(this->ui->applicationIconButton, &QPushButton::pressed, this,
                &FormMainWindow::masterVolumeMuteButtonPressed);
        connect(this->ui->settingsButton, &QPushButton::pressed, this, &FormMainWindow::settingsButtonPressed);
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
        if (this->masterVolumeLevel <= 33) {
            return QIcon(":/sound_lowest.png");
        }

        if (this->masterVolumeLevel > 33 && this->masterVolumeLevel <= 66) {
            return QIcon(":/sound_lower.png");
        }

        return QIcon(":/sound.png");
    }

    void FormMainWindow::updateComputerMasterVolume() {
        //TODO
        // audio::PulseAudioController::ChangeMasterVolume(this->masterVolumeMuted ? 0 : this->masterVolumeLevel);
        pulseAudioController->changeMasterVolume(this->masterVolumeMuted ? 0 : this-> masterVolumeLevel);
    }

    void FormMainWindow::outputDeviceChanged(int index) const {
        qDebug("INDEX %i", index);
    }

    void FormMainWindow::masterVolumeSliderChanged(float value) {
        qDebug("Master Volue: %f", value);
        this->masterVolumeLevel = value;

        this->ui->applicationIconButton->setIcon(getSpeakerIconBasedOnVolume());

        updateComputerMasterVolume();
    }

    void FormMainWindow::masterVolumeMuteButtonPressed() {
        masterVolumeMuted = !masterVolumeMuted;

        this->ui->applicationIconButton->setIcon(masterVolumeMuted ? QIcon(":/mute.png") : getSpeakerIconBasedOnVolume());

        updateComputerMasterVolume();
    }

    void FormMainWindow::settingsButtonPressed() {
        if(this->settingsWindow == nullptr)
            this->settingsWindow = new dd::forms::FormSettingsWindow();

        settingsWindow->show();
    }
} // dd::forms
