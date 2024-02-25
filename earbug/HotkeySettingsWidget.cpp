#include "HotkeySettingsWidget.h"

#include "EarBug.h"
#include "Hotkey.h"
#include "ui_HotkeySettingsWidget.h"

namespace dd::widgets::settings {
    HotkeySettingsWidget::HotkeySettingsWidget(const earbug::settings::Hotkey &hotkey,
                                               QWidget *parent) : QWidget(parent),
                                                                  ui(new Ui::HotkeySettingsWidget) {
        ui->setupUi(this);

        this->ui->hotkeyNameLabel->setText(hotkey.name);
        this->hotkey = hotkey;
        this->ui->keySequenceEdit->setKeySequence(this->hotkey.keys);
        connect(this->ui->keySequenceEdit, &QKeySequenceEdit::keySequenceChanged, this,
                &HotkeySettingsWidget::keySequenceSet);
    }

    HotkeySettingsWidget::~HotkeySettingsWidget() {
        delete ui;
    }

    void HotkeySettingsWidget::keySequenceSet(const QKeySequence &keySequence) {
        this->hotkey.keys = keySequence;
        earbug::EarBug::getApplicationSingletonPointer()->getSettingsManager()->saveHotkey(this->hotkey);
    }
} // dd::widgets::settings
