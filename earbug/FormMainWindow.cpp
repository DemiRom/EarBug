#include "FormMainWindow.h"
#include "ui_FormMainWindow.h"

#include <QAudioDevice>
#include <QMediaDevices>

#include "EarBug.h"
#include "PulseAudioController.h"
#include "FormSettingsWindow.h"
#include "uglobalhotkeys.h"

namespace dd::forms {
    FormMainWindow::FormMainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::FormMainWindow) {
        ui->setupUi(this);

        //Create objects
        this->settingsWindow = new FormSettingsWindow();
        this->pulseAudioController = new audio::PulseAudioController();

        //Set the main window flags.
        this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

        //Update the master volume slider
        this->masterVolumeLevel = this->pulseAudioController->getMasterVolume();
        this->ui->masterVolumeSlider->setValue(this->masterVolumeLevel); //TODO
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
        pulseAudioController->changeMasterVolume(this->masterVolumeMuted ? 0 : this->masterVolumeLevel);
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

        // ReShaper bug - thinks this code is unreachable for some reason.
        // ReSharper disable once CppDFAUnreachableCode
        this->ui->applicationIconButton->setIcon(
            masterVolumeMuted ? QIcon(":/mute.png") : getSpeakerIconBasedOnVolume());

        updateComputerMasterVolume();
    }

    void FormMainWindow::settingsButtonPressed() const {
        settingsWindow->show();
    }

    void FormMainWindow::applicationStateChanged(const Qt::ApplicationState state) {
        if (!this->settingsWindow->isHidden())
            return;

        if (state == Qt::ApplicationInactive)
            this->hide();
    }
} // dd::forms
