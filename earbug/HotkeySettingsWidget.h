#ifndef HOTKEYSETTINGSWIDGET_H
#define HOTKEYSETTINGSWIDGET_H

#include <QWidget>

#include "Hotkey.h"

namespace dd::widgets::settings {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class HotkeySettingsWidget;
    }

    QT_END_NAMESPACE

    class HotkeySettingsWidget : public QWidget {
        Q_OBJECT

    public:
        explicit HotkeySettingsWidget(const earbug::settings::Hotkey &hotkey, QWidget *parent = nullptr);

        ~HotkeySettingsWidget() override;

    private:
        Ui::HotkeySettingsWidget *ui;
        earbug::settings::Hotkey hotkey;

    private slots:
        void keySequenceSet(const QKeySequence &keySequence);
    };
} // dd::widgets::settings

#endif //HOTKEYSETTINGSWIDGET_H
