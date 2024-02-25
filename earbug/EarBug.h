#ifndef EARBUG_H
#define EARBUG_H
#include <QApplication>

#include "FormMainWindow.h"
#include "GlobalSettingsManager.h"


namespace dd::earbug {
    class EarBug;

    namespace Private {
        static EarBug *self;
    }

    class EarBug {
    public:
        EarBug();
        ~EarBug();

        int start(int argc, char **argv);
        void setWindowPosition() const;
        void setWindowSize() const;
        void setDisplay(const QString& displayName);
        void setTheme(const QString &theme) const;

        [[nodiscard]] settings::GlobalSettingsManager* getSettingsManager() const;

        static EarBug* getApplicationSingletonPointer();
    protected:
    private:
        QApplication *app = nullptr;
        forms::FormMainWindow *mainWindow = nullptr;

        QScreen *earbugDisplay = nullptr;

        settings::GlobalSettingsManager* settingsManager = nullptr;
    };
}
#endif //EARBUG_H
