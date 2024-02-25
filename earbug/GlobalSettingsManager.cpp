#include "GlobalSettingsManager.h"

#include <qdir.h>

#include "Hotkey.h"
#include "Theme.h"

namespace dd::forms::theme {
    struct Theme;
}

namespace dd::earbug::settings {
    GlobalSettingsManager::GlobalSettingsManager(QWidget *parent) {
        this->settings = new QSettings("DemiRom", "EarBug");
        this->hotkeyManager = new UGlobalHotkeys(parent);

        qRegisterMetaType<Hotkey>("Hotkey");

        //TODO Add all the hotkeys here
        this->hotkeys.append(getHotkey({
            "Show Earbug", QKeySequence::fromString("Ctrl+Shift+C"), 1
        }));
        this->hotkeys.append(getHotkey({
            "Master Mute", QKeySequence::fromString("Ctrl+Shift+M"), 2
        }));
        this->hotkeys.append(getHotkey({
            "Master Volume Up", QKeySequence::fromString("Ctrl+Shift+="), 3
        }));
        this->hotkeys.append(getHotkey({
            "Master Volume Down", QKeySequence::fromString("Ctrl+Shift+-"), 4
        }));
        this->hotkeys.append(getHotkey({
            "Quit", QKeySequence::fromString("Ctrl+Shift+E"), 5
        }));

        registerHotkeys();

        connect(this->hotkeyManager, &UGlobalHotkeys::activated, [this](const size_t id) {
            qDebug() << "Hotkey Pressed: " << id;
            switch(id) {
                case 1:
                    emit showEarBugHotkeyPressed();
                    break;
                case 2:
                    emit masterMuteHotkeyPressed();
                    break;
                case 3:
                    emit masterVolumeUp();
                    break;
                case 4:
                    emit masterVolumeDown();
                    break;
                case 5:
                    emit forceQuit();
                    break;
                default:
                    emit hotkeyPressed(id);
                    break;
            }
        });
    }

    GlobalSettingsManager::~GlobalSettingsManager() {
        delete hotkeyManager;
        delete settings;
    }

    void GlobalSettingsManager::saveXOffset(const int xOffset) const {
        this->settings->setValue("xOffset", xOffset);
    }

    void GlobalSettingsManager::saveYOffset(const int yOffset) const {
        this->settings->setValue("yOffset", yOffset);
    }

    void GlobalSettingsManager::saveWindowPosition(EarBugWindowPosition windowPosition) const {
        this->settings->setValue("position", static_cast<int>(windowPosition));
    }

    void GlobalSettingsManager::saveHotkey(const Hotkey &hotkey) const {
        this->settings->setValue(hotkey.name, QVariant::fromValue(hotkey));

        if (hotkey.keys.toString().isEmpty())
            return;

        //Re register the hotkey on save
        this->hotkeyManager->unregisterHotkey(hotkey.id);
        this->hotkeyManager->registerHotkey(hotkey.keys.toString(), hotkey.id);
    }

    void GlobalSettingsManager::saveTheme(const QString &theme) const {
        this->settings->setValue("theme", theme);
    }

    void GlobalSettingsManager::saveDisplay(const QString &display) const {
        this->settings->setValue("display", display);
    }

    void GlobalSettingsManager::saveShowOnCurrentDisplay(const bool showOnCurrentDisplay) const {
        this->settings->setValue("showOnCurrentDisplay", showOnCurrentDisplay);
    }

    void GlobalSettingsManager::saveWidth(const int width) const {
        this->settings->setValue("width", width);
    }

    void GlobalSettingsManager::saveHeight(const int height) const {
        this->settings->setValue("height", height);
    }

    int GlobalSettingsManager::getWidth() const {
        return this->settings->value("width").toInt();
    }

    int GlobalSettingsManager::getHeight() const {
        return this->settings->value("height").toInt();
    }

    QList<dd::settings::Theme> GlobalSettingsManager::loadThemes() {
        const auto themeDir = QDir("./themes");
        auto themes = themeDir.entryList(QStringList() << "*.theme", QDir::Files);
        auto ret = QList<dd::settings::Theme>();
        for (const auto& name : themes) {
            auto file = QFile("./themes/" + name);
            if(!file.open(QFile::ReadOnly)) {
                continue;
            }
            const auto value = QString::fromStdString(file.readAll().toStdString());
            ret.append({name, value});
            file.close();
        }
        return ret;
    }

    QString GlobalSettingsManager::getThemeDataByName(const QString &name) {
        auto file = QFile("./themes/" + name);
        if(!file.open(QFile::ReadOnly)) {
            qWarning() << "Theme file not found";
            return "";
        }
        const auto value = QString::fromStdString(file.readAll().toStdString());
        file.close();
        return value;
    }

    QString GlobalSettingsManager::getThemeData() const {
        return getThemeDataByName(this->getTheme());
    }

    bool GlobalSettingsManager::getShowOnCurrentDisplay() const {
        return this->settings->value("showOnCurrentDisplay").toBool();
    }

    void GlobalSettingsManager::resetSettings() const {
        this->settings->clear();
    }

    QString GlobalSettingsManager::getDisplay() const {
        return this->settings->value("display").toString();
    }

    QList<Hotkey> GlobalSettingsManager::getAllHotkeys() {
        return this->hotkeys;
    }

    Hotkey GlobalSettingsManager::getHotkey(const Hotkey &defaultHotkey) const {
        auto hotkey = this->settings->value(defaultHotkey.name).value<Hotkey>();

        //If the settings file does not contain data on this hotkey, assign it the default
        if (hotkey.name.isEmpty())
            return defaultHotkey;

        return hotkey;
    }

    int GlobalSettingsManager::getXOffset() const {
        return this->settings->value("xOffset").toInt();
    }

    int GlobalSettingsManager::getYOffset() const {
        return this->settings->value("yOffset").toInt();
    }

    EarBugWindowPosition GlobalSettingsManager::getEarbugWindowPosition() const {
        return static_cast<EarBugWindowPosition>(this->settings->value("position").toInt());
    }

    QString GlobalSettingsManager::getTheme() const {
        return this->settings->value("theme").toString();
    }

    void GlobalSettingsManager::registerHotkeys() {
        for (const auto &[name, keys, id]: this->hotkeys) {
            if (keys.toString().isEmpty()) {
                qDebug() << "Failed to register empty hotkey: " << name;
                continue;
            }
            hotkeyManager->registerHotkey(keys.toString(), id);
            qDebug() << "Registered Hotkey: " << name << " Hotkey Sequence: " << keys.toString();
        }
    }
}
