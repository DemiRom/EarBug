#include "FormSettingsWindow.h"

#include "EarBug.h"
#include "EarBugWindowPosition.h"
#include "GlobalSettingsManager.h"
#include "HotkeySettingsWidget.h"
#include "ui_FormSettingsWindow.h"

namespace dd::forms {
    FormSettingsWindow::FormSettingsWindow(QWidget *parent) : QWidget(parent), ui(new Ui::FormSettingsWindow) {
        ui->setupUi(this);

        loadAllSettings();

        //TODO
        for (const auto& hotkey : earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager()->getAllHotkeys()) {
            this->ui->keyBindControlsContainer->addWidget(
                new widgets::settings::HotkeySettingsWidget(hotkey));
        }

        connect(this->ui->cancelButton, &QPushButton::pressed, this, &FormSettingsWindow::close);
        connect(this->ui->saveButton, &QPushButton::pressed, this, &FormSettingsWindow::saveSettings);

        connect(this->ui->bottomLeftPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarBugBottomLeft);
        connect(this->ui->bottomRightPushButton, &QPushButton::pressed, this,
                &FormSettingsWindow::setEarBugBottomRight);
        connect(this->ui->topLeftPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarButTopLeft);
        connect(this->ui->topRightPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarButTopRight);
        connect(this->ui->resetButton, &QPushButton::pressed, this, &FormSettingsWindow::resetButtonPressed);
        this->ui->xOffsetInput->setValidator(new QIntValidator());
        this->ui->yOffsetInput->setValidator(new QIntValidator());
        connect(this->ui->selectDisplayComboBox, &QComboBox::currentTextChanged, this, &FormSettingsWindow::displayChanged);
        connect(this->ui->showOnCurrentDisplayCheckBox, &QCheckBox::stateChanged, this, &FormSettingsWindow::showOnCurrentDisplayCheckBoxChanged);
    }

    FormSettingsWindow::~FormSettingsWindow() {
        delete this->ui->xOffsetInput->validator();
        delete this->ui->yOffsetInput->validator();
        delete ui;
    }

    void FormSettingsWindow::loadAllSettings() {
        //Populate the display selection combo box
        for (const auto display: QApplication::screens()) {
            this->ui->selectDisplayComboBox->addItem(display->name());
        }

        const auto earBugAppSettingsManagerPtr = earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager();

        this->ui->xOffsetInput->setText(QString::number(earBugAppSettingsManagerPtr->getXOffset()));
        this->ui->yOffsetInput->setText(QString::number(earBugAppSettingsManagerPtr->getYOffset()));
        this->position = earBugAppSettingsManagerPtr->getEarbugWindowPosition();
        this->ui->selectDisplayComboBox->setCurrentText(earBugAppSettingsManagerPtr->getDisplay());
        this->ui->showOnCurrentDisplayCheckBox->setChecked(earBugAppSettingsManagerPtr->getShowOnCurrentDisplay());
        this->ui->selectDisplayComboBox->setDisabled(this->ui->showOnCurrentDisplayCheckBox->isChecked());

        setPositionalButtonState();
    }

    void FormSettingsWindow::setPositionalButtonState() const {
        this->ui->bottomLeftPushButton->setDisabled(
            this->position == earbug::settings::EarBugWindowPosition::BOTTOM_LEFT);
        this->ui->bottomRightPushButton->setDisabled(
            this->position == earbug::settings::EarBugWindowPosition::BOTTOM_RIGHT);
        this->ui->topLeftPushButton->setDisabled(
            this->position == earbug::settings::EarBugWindowPosition::TOP_LEFT);
        this->ui->topRightPushButton->setDisabled(
            this->position == earbug::settings::EarBugWindowPosition::TOP_RIGHT);
    }

    void FormSettingsWindow::updateWindowPosition() {
        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        earBugAppPtr->setWindowPosition();
    }

    void FormSettingsWindow::showOnCurrentDisplayCheckBoxChanged(int state) const {
        this->ui->selectDisplayComboBox->setDisabled(state);
    }

    void FormSettingsWindow::displayChanged(const QString &selectedDisplay) {
        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        earBugAppPtr->setDisplay(selectedDisplay);
    }

    void FormSettingsWindow::saveSettings() {
        const auto earBugAppSettingsManagerPtr = earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager();
        earBugAppSettingsManagerPtr->saveXOffset(QVariant::fromValue(this->ui->xOffsetInput->text()).toInt());
        earBugAppSettingsManagerPtr->saveYOffset(QVariant::fromValue(this->ui->yOffsetInput->text()).toInt());
        earBugAppSettingsManagerPtr->saveWindowPosition(this->position);
        earBugAppSettingsManagerPtr->saveDisplay(this->ui->selectDisplayComboBox->currentText());
        earBugAppSettingsManagerPtr->saveShowOnCurrentDisplay(this->ui->showOnCurrentDisplayCheckBox->isChecked());

        updateWindowPosition();

        this->close();
    }

    void FormSettingsWindow::setEarBugBottomLeft() {
        this->position = earbug::settings::EarBugWindowPosition::BOTTOM_LEFT;
        setPositionalButtonState();
    }

    void FormSettingsWindow::setEarBugBottomRight() {
        this->position = earbug::settings::EarBugWindowPosition::BOTTOM_RIGHT;
        setPositionalButtonState();
    }

    void FormSettingsWindow::setEarButTopLeft() {
        this->position = earbug::settings::EarBugWindowPosition::TOP_LEFT;
        setPositionalButtonState();
    }

    void FormSettingsWindow::setEarButTopRight() {
        this->position = earbug::settings::EarBugWindowPosition::TOP_RIGHT;
        setPositionalButtonState();
    }

    void FormSettingsWindow::resetButtonPressed() {
        const auto earButAppSettingsManagerPtr = earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager();
        earButAppSettingsManagerPtr->resetSettings();
    }
} // dd::forms
