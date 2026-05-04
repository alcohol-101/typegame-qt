#ifndef RESPATH_H
#define RESPATH_H

#include <QCoreApplication>
#include <QString>
#include <QUrl>
#include <QResource>
#include <QDebug>

inline QString resPath(const QString& relativePath)
{
    return QCoreApplication::applicationDirPath() + "/" + relativePath;
}

inline QUrl resUrl(const QString& relativePath)
{
    return QUrl::fromLocalFile(resPath(relativePath));
}

inline bool ensureGameResources()
{
    static bool registered = false;
    if (registered)
        return true;

    QString rccPath = resPath("res_game.rcc");
    if (!QResource::registerResource(rccPath)) {
        qWarning() << "Failed to register game resources from:" << rccPath;
        return false;
    }
    registered = true;
    qDebug() << "Game resources registered from:" << rccPath;
    return true;
}

#endif // RESPATH_H
