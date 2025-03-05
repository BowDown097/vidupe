#include "osutils.h"
#include <QDir>

namespace OSUtils
{
    QString getFullPath(const QFileInfo& fileInfo)
    {
        QString absoluteFilePath = fileInfo.absoluteFilePath();
        if (fileInfo.exists())
            return absoluteFilePath;

    #ifdef Q_OS_WIN
        absoluteFilePath += ".exe";
        if (QFile::exists(absoluteFilePath))
            return absoluteFilePath;
    #endif

        if (const QString pathEnv = qgetenv("PATH"); !pathEnv.isEmpty())
        {
            const QStringList pathParts = pathEnv.split(QDir::listSeparator());
            for (const QString& dir : pathParts)
            {
                QString fullPath = dir + QDir::separator() + fileInfo.fileName();
                if (QFile::exists(fullPath))
                    return fullPath;

            #ifdef Q_OS_WIN
                fullPath += ".exe";
                if (QFile::exists(fullPath))
                    return fullPath;
            #endif
            }
        }

        return QString();
    }
}
