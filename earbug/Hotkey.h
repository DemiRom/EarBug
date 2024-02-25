#ifndef HOTKEYS_H
#define HOTKEYS_H
#include <qkeysequence.h>

namespace dd::earbug::settings {
    struct Hotkey {
        QString name;
        QKeySequence keys;
        qsizetype id = 1;
    };



}

inline QDataStream& operator<<(QDataStream& out, const dd::earbug::settings::Hotkey& v) {
    out << v.name << v.keys << v.id;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, dd::earbug::settings::Hotkey& v) {
    in >> v.name;
    in >> v.keys;
    in >> v.id;
    return in;
}

Q_DECLARE_METATYPE(dd::earbug::settings::Hotkey);

#endif //HOTKEYS_H
