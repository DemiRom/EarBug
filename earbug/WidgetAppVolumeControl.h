#ifndef WIDGETAPPVOLUMECONTROL_H
#define WIDGETAPPVOLUMECONTROL_H

#include <QWidget>

namespace dd::widgets {
QT_BEGIN_NAMESPACE
namespace Ui { class WidgetAppVolumeControl; }
QT_END_NAMESPACE

class WidgetAppVolumeControl : public QWidget {
Q_OBJECT

public:
    explicit WidgetAppVolumeControl(QWidget *parent = nullptr);
    ~WidgetAppVolumeControl() override;

private:
    Ui::WidgetAppVolumeControl *ui;
};
} // dd::widgets

#endif //WIDGETAPPVOLUMECONTROL_H
