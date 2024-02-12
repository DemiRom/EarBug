#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSettings>

namespace dd::forms {
QT_BEGIN_NAMESPACE
namespace Ui { class FormSettingsWindow; }
QT_END_NAMESPACE

class FormSettingsWindow : public QWidget {
Q_OBJECT

public:
    explicit FormSettingsWindow(QWidget *parent = nullptr);
    ~FormSettingsWindow() override;

private:
    Ui::FormSettingsWindow *ui;
    QSettings* earBugSettings;

    void loadAllSettings() const;

private slots:
    void saveSettings();
    void setEarBugBottomLeft();
    void setEarBugBottomRight();
    void setEarButTopLeft();
    void setEarButTopRight();
};
} // dd::forms

#endif //SETTINGSWINDOW_H
