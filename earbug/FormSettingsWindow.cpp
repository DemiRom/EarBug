#include "FormSettingsWindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "EarBug.h"
#include "EarBugWindowPosition.h"
#include "GlobalSettingsManager.h"
#include "HotkeySettingsWidget.h"
#include "ui_FormSettingsWindow.h"

namespace dd::forms {
    FormSettingsWindow::FormSettingsWindow(QWidget *parent) : QWidget(parent), ui(new Ui::FormSettingsWindow) {
        ui->setupUi(this);

        loadAllSettings();

        for (const auto &hotkey: earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager()->
            getAllHotkeys()) {
            this->ui->keyBindControlsContainer->addWidget(
                new widgets::settings::HotkeySettingsWidget(hotkey));
        }

        this->ui->xOffsetInput->setValidator(new QIntValidator());
        this->ui->yOffsetInput->setValidator(new QIntValidator());
        this->ui->widthLineEdit->setValidator(new QIntValidator());
        this->ui->heightLineEdit->setValidator(new QIntValidator());

        connect(this->ui->cancelButton, &QPushButton::pressed, this, &FormSettingsWindow::close);
        connect(this->ui->saveButton, &QPushButton::pressed, this, &FormSettingsWindow::saveSettings);
        connect(this->ui->bottomLeftPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarBugBottomLeft);
        connect(this->ui->bottomRightPushButton, &QPushButton::pressed, this,
                &FormSettingsWindow::setEarBugBottomRight);
        connect(this->ui->topLeftPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarButTopLeft);
        connect(this->ui->topRightPushButton, &QPushButton::pressed, this, &FormSettingsWindow::setEarButTopRight);
        connect(this->ui->resetButton, &QPushButton::pressed, this, &FormSettingsWindow::resetButtonPressed);
        connect(this->ui->selectDisplayComboBox, &QComboBox::currentTextChanged, this,
                &FormSettingsWindow::displayChanged);
        connect(this->ui->showOnCurrentDisplayCheckBox, &QCheckBox::stateChanged, this,
                &FormSettingsWindow::showOnCurrentDisplayCheckBoxChanged);
        connect(this->ui->xOffsetInput, &QLineEdit::textChanged, this, &FormSettingsWindow::xOffsetChanged);
        connect(this->ui->yOffsetInput, &QLineEdit::textChanged, this, &FormSettingsWindow::yOffsetChanged);
        connect(this->ui->widthLineEdit, &QLineEdit::textChanged, this, &FormSettingsWindow::widthChanged);
        connect(this->ui->heightLineEdit, &QLineEdit::textChanged, this, &FormSettingsWindow::heightChanged);
        connect(this->ui->themeComboBox, &QComboBox::currentIndexChanged, this, &FormSettingsWindow::themeChanged);

        connect(this->ui->addThemeButton, &QPushButton::pressed, this, &FormSettingsWindow::addThemeButtonPressed);
        connect(this->ui->deleteThemeButton, &QPushButton::pressed, this, &FormSettingsWindow::deleteThemeButtonPressed);
    }

    FormSettingsWindow::~FormSettingsWindow() {
        delete this->ui->xOffsetInput->validator();
        delete this->ui->yOffsetInput->validator();
        delete ui;
    }

    void FormSettingsWindow::loadAllSettings() {
        populateDisplayComboBox();

        const auto earBugAppSettingsManagerPtr = earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager();

        this->ui->xOffsetInput->setText(QString::number(earBugAppSettingsManagerPtr->getXOffset()));
        this->ui->yOffsetInput->setText(QString::number(earBugAppSettingsManagerPtr->getYOffset()));
        this->ui->widthLineEdit->setText(QString::number(earBugAppSettingsManagerPtr->getWidth()));
        this->ui->heightLineEdit->setText(QString::number(earBugAppSettingsManagerPtr->getHeight()));
        this->position = earBugAppSettingsManagerPtr->getEarbugWindowPosition();
        this->ui->selectDisplayComboBox->setCurrentText(earBugAppSettingsManagerPtr->getDisplay());
        this->ui->showOnCurrentDisplayCheckBox->setChecked(earBugAppSettingsManagerPtr->getShowOnCurrentDisplay());
        this->ui->selectDisplayComboBox->setDisabled(this->ui->showOnCurrentDisplayCheckBox->isChecked());
        //TODO Check if the theme still exists
        this->ui->themeComboBox->setCurrentText(earBugAppSettingsManagerPtr->getTheme());

        setPositionalButtonState();
    }

    void FormSettingsWindow::populateDisplayComboBox() const {
        //Populate the display selection combo box
        this->ui->selectDisplayComboBox->clear();
        for (const auto display: QApplication::screens()) {
            this->ui->selectDisplayComboBox->addItem(display->name());
        }
    }

    void FormSettingsWindow::populateThemeComboBox() const {
        const auto earBugAppSettingsManagerPtr = earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager();

        this->ui->themeComboBox->clear();
        for (const auto& [name, theme] : earBugAppSettingsManagerPtr->loadThemes()) {
            this->ui->themeComboBox->addItem(name, theme);
        }
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

    void FormSettingsWindow::updateWindowPositionOnPositionChanged() const {
        const auto earBugAppSettingsManagerPtr = earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager();
        earBugAppSettingsManagerPtr->saveWindowPosition(this->position);
        updateWindowPosition();
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

        if(this->ui->widthLineEdit->text().toInt() >= 400) {
            //Force no less than 400px in width
            earBugAppSettingsManagerPtr->saveWidth(QVariant::fromValue(this->ui->widthLineEdit->text()).toInt());
        }

        if(this->ui->heightLineEdit->text().toInt() >= 300) {
            //Force no less than 300px in height
            earBugAppSettingsManagerPtr->saveHeight(QVariant::fromValue(this->ui->heightLineEdit->text()).toInt());
        }

        earBugAppSettingsManagerPtr->saveXOffset(QVariant::fromValue(this->ui->xOffsetInput->text()).toInt());
        earBugAppSettingsManagerPtr->saveYOffset(QVariant::fromValue(this->ui->yOffsetInput->text()).toInt());
        earBugAppSettingsManagerPtr->saveWindowPosition(this->position);
        earBugAppSettingsManagerPtr->saveDisplay(this->ui->selectDisplayComboBox->currentText());
        earBugAppSettingsManagerPtr->saveShowOnCurrentDisplay(this->ui->showOnCurrentDisplayCheckBox->isChecked());
        earBugAppSettingsManagerPtr->saveTheme(this->ui->themeComboBox->currentText());
        updateWindowPosition();

        this->close();
    }

    void FormSettingsWindow::setEarBugBottomLeft() {
        this->position = earbug::settings::EarBugWindowPosition::BOTTOM_LEFT;
        setPositionalButtonState();
        updateWindowPositionOnPositionChanged();
    }

    void FormSettingsWindow::setEarBugBottomRight() {
        this->position = earbug::settings::EarBugWindowPosition::BOTTOM_RIGHT;
        setPositionalButtonState();
        updateWindowPositionOnPositionChanged();
    }

    void FormSettingsWindow::setEarButTopLeft() {
        this->position = earbug::settings::EarBugWindowPosition::TOP_LEFT;
        setPositionalButtonState();
        updateWindowPositionOnPositionChanged();
    }

    void FormSettingsWindow::setEarButTopRight() {
        this->position = earbug::settings::EarBugWindowPosition::TOP_RIGHT;
        setPositionalButtonState();
        updateWindowPositionOnPositionChanged();
    }

    void FormSettingsWindow::resetButtonPressed() {
        const auto earButAppSettingsManagerPtr = earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager();
        earButAppSettingsManagerPtr->resetSettings();
    }

    void FormSettingsWindow::xOffsetChanged(const QString &newText) const {
        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        earBugAppPtr->getSettingsManager()->saveXOffset(QVariant::fromValue(this->ui->xOffsetInput->text()).toInt());
        updateWindowPosition();
    }

    void FormSettingsWindow::yOffsetChanged(const QString &newText) const {
        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        earBugAppPtr->getSettingsManager()->saveYOffset(QVariant::fromValue(this->ui->yOffsetInput->text()).toInt());
        updateWindowPosition();
    }

    void FormSettingsWindow::widthChanged(const QString &newText) const {
        if(newText.toInt() < 400) {
            //Force no less than 400px in width
            return;
        }

        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        earBugAppPtr->getSettingsManager()->saveWidth(QVariant::fromValue(this->ui->widthLineEdit->text()).toInt());
        earBugAppPtr->setWindowSize();
    }

    void FormSettingsWindow::themeChanged(int idx) const {
        //TODO Update the theme here
        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        earBugAppPtr->getSettingsManager()->saveTheme(this->ui->themeComboBox->currentText());
        earBugAppPtr->setTheme(this->ui->themeComboBox->currentData().toString());
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void FormSettingsWindow::addThemeButtonPressed() const { // NOLINT(*-convert-member-functions-to-static)
        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        const auto fileName = QFileDialog::getOpenFileName(nullptr, "Open your theme", QDir::homePath(), "CSS files (*.css);;Theme files (*.theme);;All Files (*.*)");
        if (!fileName.isEmpty()) {
            earBugAppPtr->getSettingsManager()->addTheme(fileName);
        }
        populateThemeComboBox();
    }

    void FormSettingsWindow::deleteThemeButtonPressed() const {
        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        earBugAppPtr->getSettingsManager()->deleteTheme(this->ui->themeComboBox->currentText());
        populateThemeComboBox();
    }

    void FormSettingsWindow::showEvent(QShowEvent *event) {
        this->loadAllSettings();
        QWidget::showEvent(event);
    }

    void FormSettingsWindow::heightChanged(const QString &newText) const {
        if(newText.toInt() < 300) {
            //Force no less than 300px in height
            return;
        }

        const auto earBugAppPtr = earbug::EarBug::getApplicationSingletonPointer();
        earBugAppPtr->getSettingsManager()->saveHeight(QVariant::fromValue(this->ui->heightLineEdit->text()).toInt());
        earBugAppPtr->setWindowSize();
    }
} // dd::forms
