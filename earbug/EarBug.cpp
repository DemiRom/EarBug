#include "EarBug.h"

#include <QApplication>

#include "DebugLogger.h"
#include "EarBugWindowPosition.h"
#include "FormMainWindow.h"

namespace dd::earbug {
    EarBug::EarBug() {
        Private::self = this;

#ifdef NDEBUG
        qInstallMessageHandler(logging::RedirectDebugMessagesToFile);
#endif
    }

    EarBug::~EarBug() {
        delete settingsManager;
        delete mainWindow;
        delete app;
    }

    int EarBug::start(int argc, char **argv) {
        this->app = new QApplication(argc, argv);
        this->settingsManager = new settings::GlobalSettingsManager();
        this->mainWindow = new forms::FormMainWindow();
        this->app->setStyleSheet(getSettingsManager()->getThemeData());

        // Get the primary screen to start in case there is no valid display
        this->earbugDisplay = QApplication::primaryScreen();
        setDisplay(this->settingsManager->getDisplay());
        setWindowPosition();
        setWindowSize();

        this->mainWindow->show();

        return QApplication::exec();
    }

    void EarBug::setWindowPosition() const {
        if(this->earbugDisplay == nullptr) {
            qWarning() << "Display Ptr is NULL";
            return;
        }

        // Get the screen resolution
        const QRect screenResolution = this->earbugDisplay->geometry();


        // Output the screen resolution
        qDebug() << "Screen Resolution Top: " << screenResolution.top() << "Left: " << screenResolution.left() <<
                " Width: " << screenResolution.width() << "Height:" << screenResolution.height();

        int offsetX = getSettingsManager()->getXOffset();
        int offsetY = getSettingsManager()->getYOffset();

        //Fix bug that is displayed on wrong monitor
        offsetX = offsetX + screenResolution.left();
        offsetY = offsetY + screenResolution.top();

        switch (getSettingsManager()->getEarbugWindowPosition()) {
            case settings::EarBugWindowPosition::TOP_RIGHT:
                mainWindow->move(screenResolution.width() - mainWindow->geometry().width() + (-offsetX), offsetY);
                break;
            case settings::EarBugWindowPosition::TOP_LEFT:
                mainWindow->move(offsetX, offsetY);
                break;
            case settings::EarBugWindowPosition::BOTTOM_LEFT:
                mainWindow->move(offsetX, screenResolution.height() - mainWindow->geometry().height() + (-offsetY));
                break;
            case settings::EarBugWindowPosition::BOTTOM_RIGHT:
                mainWindow->move(screenResolution.width() - mainWindow->geometry().width() + (-offsetX),
                                 screenResolution.height() - mainWindow->geometry().height() + (-offsetY));
                break;
            default: ;
        }
    }

    void EarBug::setWindowSize() const {
        if(this->settingsManager->getWidth() == 0) {
            this->settingsManager->saveWidth(this->mainWindow->geometry().width());
        }
        if(this->settingsManager->getHeight() == 0) {
            this->settingsManager->saveHeight(this->mainWindow->geometry().height());
        }

        const int width = this->settingsManager->getWidth();
        const int height = this->settingsManager->getHeight();
        this->mainWindow->setFixedSize(width, height);
    }

    void EarBug::setDisplay(const QString& displayName) {
        QScreen* newScreen = nullptr;
        for(const auto& screen : QApplication::screens()) {
            if(screen->name() == displayName)
                newScreen = screen;
        }

        if(newScreen == nullptr) {
            qCritical() << "ERROR Screen could not be found!";
            return;
        }

        this->earbugDisplay = newScreen;
        setWindowPosition();
    }

    void EarBug::setTheme(const QString& theme) const {
        this->app->setStyleSheet(theme);
    }

    settings::GlobalSettingsManager *EarBug::getSettingsManager() const {
        return this->settingsManager;
    }

    EarBug *EarBug::getApplicationSingletonPointer() {
        return Private::self;
    }
}
