#include "WidgetAppVolumeControl.h"
#include "ui_WidgetAppVolumeControl.h"

namespace dd::widgets {
WidgetAppVolumeControl::WidgetAppVolumeControl(QWidget *parent) :
    QWidget(parent), ui(new Ui::WidgetAppVolumeControl) {
    ui->setupUi(this);
}

WidgetAppVolumeControl::~WidgetAppVolumeControl() {
    delete ui;
}
} // dd::widgets
