#include "FormSettingsWindow.h"
#include "ui_FormSettingsWindow.h"

namespace dd::forms {
    FormSettingsWindow::FormSettingsWindow(QWidget *parent) : QWidget(parent), ui(new Ui::FormSettingsWindow) {
        ui->setupUi(this);

        earBugSettings = new QSettings("DemiRom", "EarBug");

        loadAllSettings();

        connect(this->ui->cancelButton, &QPushButton::pressed, this, &FormSettingsWindow::close);
        connect(this->ui->saveButton, &QPushButton::pressed, this, &FormSettingsWindow::saveSettings);

        connect(this->ui->bottomLeftPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarBugBottomLeft);
        connect(this->ui->bottomRightPushButton, &QPushButton::pressed, this,
                &FormSettingsWindow::setEarBugBottomRight);
        connect(this->ui->topLeftPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarButTopLeft);
        connect(this->ui->topRightPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarButTopRight);
    }

    FormSettingsWindow::~FormSettingsWindow() {
        delete earBugSettings;
        delete ui;
    }

    void FormSettingsWindow::loadAllSettings() const {
        this->ui->xOffsetInput->setText(earBugSettings->value("xOffset", 0).toString());
        this->ui->yOffsetInput->setText(earBugSettings->value("yOffset", 0).toString());
    }

    void FormSettingsWindow::saveSettings() {
        earBugSettings->setValue("xOffset", this->ui->xOffsetInput->text());
        earBugSettings->setValue("yOffset", this->ui->yOffsetInput->text());

        this->close();
    }

    void FormSettingsWindow::setEarBugBottomLeft() {
    }

    void FormSettingsWindow::setEarBugBottomRight() {
    }

    void FormSettingsWindow::setEarButTopLeft() {
    }

    void FormSettingsWindow::setEarButTopRight() {
    }
} // dd::forms
