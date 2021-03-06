/***************************************************************************
 *   Copyright (C) 2009 by Pino Toscano <pino@kde.org>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "shellutils.h"

// qt/kde includes
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QtCore/qcommandlineparser.h>

namespace ShellUtils
{

namespace detail
{

bool qfileExistFunc( const QString& fileName )
{
    return QFile::exists( fileName );
}

}

FileExistFunc qfileExistFunc()
{
    return detail::qfileExistFunc;
}

QUrl urlFromArg( const QString& _arg, FileExistFunc exist_func, const QString& pageArg )
{
    // ## TODO remove exist_func
#if QT_VERSION >= 0x050400
    QUrl url = QUrl::fromUserInput(_arg, QDir::currentPath());
#else
    // Code from QUrl::fromUserInput(QString, QString)
    QUrl url = QUrl::fromUserInput(_arg);
    QUrl testUrl = QUrl(_arg, QUrl::TolerantMode);
    if (testUrl.isRelative() && !QDir::isAbsolutePath(_arg)) {
        QFileInfo fileInfo(QDir::current(), _arg);
        if (fileInfo.exists())
            url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
    }

#endif
    if ( !pageArg.isEmpty() )
    {
      url.setFragment( pageArg );
    }
    return url;
}

QString serializeOptions(const QCommandLineParser &args)
{
    const bool startInPresentation = args.isSet( "presentation" );
    const bool showPrintDialog = args.isSet( "print" );
    const bool unique = args.isSet("unique") && args.positionalArguments().count() <= 1;
    const bool noRaise = !args.isSet("raise");
    const QString page = args.value("page");

    return serializeOptions(startInPresentation, showPrintDialog, unique, noRaise, page);
}

QString serializeOptions(bool startInPresentation, bool showPrintDialog, bool unique, bool noRaise, const QString &page)
{
    return QString("%1:%2:%3:%4:%5").arg(startInPresentation).arg(showPrintDialog).arg(unique).arg(noRaise).arg(page);
}

bool unserializeOptions(const QString &serializedOptions, bool *presentation, bool *print, bool *unique, bool *noraise, QString *page)
{
    const QStringList args = serializedOptions.split(":");
    if (args.count() == 5)
    {
        *presentation = args[0] == "1";
        *print = args[1] == "1";
        *unique = args[2] == "1";
        *noraise = args[3] == "1";
        *page = args[4];
        return true;
    }
    return false;
}

bool startInPresentation(const QString &serializedOptions)
{
    bool result, dummy;
    QString dummyString;
    return unserializeOptions(serializedOptions, &result, &dummy, &dummy, &dummy, &dummyString) && result;
}

bool showPrintDialog(const QString &serializedOptions)
{
    bool result, dummy;
    QString dummyString;
    return unserializeOptions(serializedOptions, &dummy, &result, &dummy, &dummy, &dummyString) && result;
}

bool unique(const QString &serializedOptions)
{
    bool result, dummy;
    QString dummyString;
    return unserializeOptions(serializedOptions, &dummy, &dummy, &result, &dummy, &dummyString) && result;
}

bool noRaise(const QString &serializedOptions)
{
    bool result, dummy;
    QString dummyString;
    return unserializeOptions(serializedOptions, &dummy, &dummy, &dummy, &result, &dummyString) && result;
}

QString page(const QString &serializedOptions)
{
    QString result;
    bool dummy;
    unserializeOptions(serializedOptions, &dummy, &dummy, &dummy, &dummy, &result);
    return result;
}

}
