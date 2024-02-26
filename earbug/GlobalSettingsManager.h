#ifndef GLOBALSETTINGSMANAGER_H
#define GLOBALSETTINGSMANAGER_H

#include <qdir.h>
#include <QSettings>

#include "EarBugWindowPosition.h"
#include "Hotkey.h"
#include "Theme.h"
#include "uglobalhotkeys.h"


namespace dd::earbug::settings {
    class GlobalSettingsManager : public QObject {
        Q_OBJECT

    public:
        explicit GlobalSettingsManager(QWidget *parent = nullptr);

        ~GlobalSettingsManager() override;

        void saveXOffset(int xOffset) const;

        void saveYOffset(int yOffset) const;

        void saveWindowPosition(EarBugWindowPosition windowPosition) const;

        void saveHotkey(const Hotkey &hotkey) const;

        void addTheme(const QString &fileName) const;

        void deleteTheme(const QString &fileName) const;

        void saveTheme(const QString &theme) const;

        void saveDisplay(const QString &display) const;

        void saveShowOnCurrentDisplay(bool showOnCurrentDisplay) const;

        void saveWidth(int width) const;

        void saveHeight(int height) const;

        void resetSettings() const;

        QList<Hotkey> getAllHotkeys();

        [[nodiscard]] Hotkey getHotkey(const Hotkey &defaultHotkey) const;

        [[nodiscard]] int getXOffset() const;

        [[nodiscard]] int getYOffset() const;

        [[nodiscard]] EarBugWindowPosition getEarbugWindowPosition() const;

        [[nodiscard]] QString getTheme() const;

        [[nodiscard]] QString getDisplay() const;

        [[nodiscard]] bool getShowOnCurrentDisplay() const;

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] QList<dd::settings::Theme> loadThemes() const;

        [[nodiscard]] QString getThemeDataByName(const QString& name) const;

        [[nodiscard]] QString getThemeData() const;

    signals:
        void showEarBugHotkeyPressed();
        void masterMuteHotkeyPressed();
        void forceQuit();
        void masterVolumeUp();
        void masterVolumeDown();
        void hotkeyPressed(size_t id);
        void showSettings();

    protected:
    private:
        void registerHotkeys();

        QList<Hotkey> hotkeys;

        QSettings *settings = nullptr;
        UGlobalHotkeys *hotkeyManager = nullptr;

        const QString themeDirectory = QDir::homePath() + "/.config/EarBug/themes";
    };
}

#endif //GLOBALSETTINGSMANAGER_H
