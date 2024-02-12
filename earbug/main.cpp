#include <QApplication>
#include <QScreen>

#include "FormMainWindow.h"
#include "BaseTheme.h"

using namespace dd::forms;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    a.setStyleSheet(theme::Themes[static_cast<int>(theme::ThemeType::Default)].theme);

    // Get the primary screen
    QScreen *primaryScreen = QApplication::primaryScreen();

    // Get the screen resolution
    QRect screenResolution = primaryScreen->geometry();

    // Output the screen resolution
    qDebug() << "Screen Resolution:" << screenResolution.width() << "x" << screenResolution.height();

    FormMainWindow window;
    // window.move(screenResolution.width() - window.geometry().width() - 40, 40);
    window.move(0,0);
    window.show();

    return QApplication::exec();
}