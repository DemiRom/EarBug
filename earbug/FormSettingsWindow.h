#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSettings>

#include "EarBugWindowPosition.h"

namespace dd::forms {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class FormSettingsWindow;
    }

    QT_END_NAMESPACE

    class FormSettingsWindow : public QWidget {
        Q_OBJECT

    public:
        explicit FormSettingsWindow(QWidget *parent = nullptr);

        ~FormSettingsWindow() override;

    private:
        Ui::FormSettingsWindow *ui;
        earbug::settings::EarBugWindowPosition position{};

        void loadAllSettings();

        void setPositionalButtonState() const;

        static void updateWindowPosition();

    private slots:
        void showOnCurrentDisplayCheckBoxChanged(int state) const;

        static void displayChanged(const QString& selectedDisplay);

        void saveSettings();

        void setEarBugBottomLeft();

        void setEarBugBottomRight();

        void setEarButTopLeft();

        void setEarButTopRight();

        static void resetButtonPressed();
    };
} // dd::forms

#endif //SETTINGSWINDOW_H
