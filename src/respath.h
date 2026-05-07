// filename: respath.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Resource path utilities and runtime .rcc registration

#ifndef _TYPEGAME_RESPATH_H_
#define _TYPEGAME_RESPATH_H_

#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QUrl>
#include <QResource>
#include <QDebug>

inline QString ResPath(const QString& relativePath)
{
    return QCoreApplication::applicationDirPath() + QDir::separator() + relativePath;
}

inline QUrl ResUrl(const QString& relativePath)
{
    return QUrl::fromLocalFile(ResPath(relativePath));
}

inline bool EnsureGameResources()
{
    static bool registered = false;
    if (registered)
        return true;

    QString rccPath = ResPath("res_game.rcc");
    if (!QResource::registerResource(rccPath)) {
        qWarning() << "Failed to register game resources from:" << rccPath;
        return false;
    }
    registered = true;
    qDebug() << "Game resources registered from:" << rccPath;
    return true;
}

#endif // _TYPEGAME_RESPATH_H_
