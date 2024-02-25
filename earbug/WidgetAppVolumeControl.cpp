#include "WidgetAppVolumeControl.h"

#include <qfile.h>
#include <utility>
#include "ui_WidgetAppVolumeControl.h"

namespace dd::widgets {
    WidgetAppVolumeControl::WidgetAppVolumeControl(audio::PulseAudioController *audioContoller, audio::Sink *sinkItem,
                                                   QWidget *parent) : QWidget(parent),
                                                                      ui(new Ui::WidgetAppVolumeControl),
                                                                      sinkItem(sinkItem),
                                                                      audioController(audioContoller) {
        ui->setupUi(this);

        assert(audioContoller != nullptr);
        assert(sinkItem != nullptr);

        this->setWhatsThis(QString::fromStdString(sinkItem->name));

        this->ui->volumeSlider->setMaximum(PA_VOLUME_UI_MAX);
        this->ui->volumeSlider->setValue(this->audioController->getVolume(this->sinkItem->idx));

        this->clientName = QString::fromStdString(this->sinkItem->client_name);

        //TODO Get mute state
        this->muted = this->sinkItem->muted;

        this->ui->iconAndMuteButton->setIcon(getSpeakerIconBasedOnVolume());

        connect(this->ui->volumeSlider, &QSlider::valueChanged, this, &WidgetAppVolumeControl::volumeChanged);
        connect(this->ui->iconAndMuteButton, &QPushButton::pressed, this, &WidgetAppVolumeControl::muteButtonPressed);
    }

    WidgetAppVolumeControl::~WidgetAppVolumeControl() {
        delete ui;
    }

    void WidgetAppVolumeControl::volumeChanged(const int value) const {
        qDebug() << "Value: " << value;
        this->audioController->setVolume(this->sinkItem->idx, value);
        this->ui->iconAndMuteButton->setIcon(getSpeakerIconBasedOnVolume());
    }

    //TODO Refactor this into a proper class.
    QString tryFindIconPath(const QString& appName) {
        if(appName.isEmpty()) {
            return "";
        }

        const auto desktopEntryPath = QString("/usr/share/applications/%1.desktop").arg(appName);
        QFile file(desktopEntryPath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Cannot open file" << desktopEntryPath;
            return "";
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("Icon=")) {
                return line.mid(5).trimmed(); // Extract the icon path
            }
        }

        return "";
    }

    QIcon WidgetAppVolumeControl::getSpeakerIconBasedOnVolume() const {
        if (this->muted) {
            return QIcon(":/mute.png");
        }

        QIcon themeIcon = QIcon::fromTheme(this->clientName.toLower());

        if(!themeIcon.isNull()) {
            return themeIcon;
        }

        qDebug() << "Icon not found using default";

        if (this->ui->volumeSlider->value() <= (PA_VOLUME_UI_MAX / 3)) {
            return QIcon(":/sound_lowest.png");
        }

        if (this->ui->volumeSlider->value() > (PA_VOLUME_UI_MAX / 3) && this->ui->volumeSlider->value() <= (
                PA_VOLUME_UI_MAX / 3) * 2) {
            return QIcon(":/sound_lower.png");
        }

        return QIcon(":/sound.png");
    }

    void WidgetAppVolumeControl::muteButtonPressed() {
        this->muted = !this->muted;
        this->audioController->toggleMute(this->sinkItem->idx);
        this->ui->iconAndMuteButton->setIcon(getSpeakerIconBasedOnVolume());
    }
} // dd::widgets
