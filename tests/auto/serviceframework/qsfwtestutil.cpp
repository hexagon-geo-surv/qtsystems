/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "qsfwtestutil.h"

#include <QDir>
#include <QFileInfoList>
#include <QFile>
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

#ifdef QT_ADDON_JSONDB_LIB
#include <QSignalSpy>
#include <QtJsonDb/qjsondbconnection.h>
#include <QtJsonDb/qjsondbreadrequest.h>
#include <QtJsonDb/qjsondbwriterequest.h>

Q_DECLARE_METATYPE(QtJsonDb::QJsonDbRequest::ErrorCode)

QT_USE_NAMESPACE_JSONDB

#endif

void QSfwTestUtil::setupTempUserDb()
{
    QSettings::setUserIniPath(tempUserDbDir());
}

void QSfwTestUtil::setupTempSystemDb()
{
    QSettings::setSystemIniPath(tempSystemDbDir());
}

void QSfwTestUtil::removeTempUserDb()
{
    removeDirectory(tempUserDbDir());
}

void QSfwTestUtil::removeTempSystemDb()
{
    removeDirectory(tempSystemDbDir());
}

QString QSfwTestUtil::tempUserDbDir()
{
    return tempSettingsPath("__user__");
}

QString QSfwTestUtil::tempSystemDbDir()
{
    return tempSettingsPath("__system__");
}

QString QSfwTestUtil::userDirectory()
{
    return tempSettingsPath("__user__/");
}

QString QSfwTestUtil::systemDirectory()
{
    return tempSettingsPath("__system__/");
}

QString QSfwTestUtil::tempSettingsPath(const char *path)
{
    // Temporary path for files that are specified explictly in the constructor.
    //QString tempPath = QDir::tempPath();
    QString tempPath = QCoreApplication::applicationDirPath();
    if (tempPath.endsWith("/"))
        tempPath.truncate(tempPath.size() - 1);
    return QDir::toNativeSeparators(tempPath + "/QtServiceFramework_tests/" + QLatin1String(path));
}

void QSfwTestUtil::removeDirectory(const QString &path)
{
    QDir dir(path);
    QFileInfoList fileList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    foreach(QFileInfo file, fileList) {
        if(file.isFile()) {
            QFile::remove (file.canonicalFilePath());
        }
        if(file.isDir()) {
            QFile::Permissions perms = QFile::permissions(file.canonicalFilePath());
            perms = perms | QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner;
            QFile::setPermissions(file.canonicalFilePath(), perms);
            removeDirectory(file.canonicalFilePath());
        }
    }
    dir.rmpath(path);
}

#if defined(QT_ADDON_JSONDB_LIB)

void QSfwTestUtil::clearDatabases_jsondb()
{

    qRegisterMetaType<QtJsonDb::QJsonDbRequest::ErrorCode>("QtJsonDb::QJsonDbRequest::ErrorCode");

    QJsonDbConnection *db = QJsonDbConnection::defaultConnection();
    db->connectToServer();

    bool waiting = true;

    QList<QJsonObject> args;

    QJsonDbReadRequest request;
    request.setQuery(QStringLiteral("[?_type=\"com.nokia.mt.serviceframework.interface\"]"));

    QSignalSpy response(&request, SIGNAL(finished()));
    QSignalSpy error(&request, SIGNAL(error(QtJsonDb::QJsonDbRequest::ErrorCode,QString)));
    QSignalSpy discon(db, SIGNAL(disconnected()));

    db->send(&request);
    do {
        QCoreApplication::processEvents();
        if (response.count()){
            args = request.takeResults();
            waiting = false;
        }
        if (error.count() || discon.count()) {
            waiting = false;
        }
    } while (waiting);

    if (args.isEmpty()) {
        return;
    }

    QJsonDbRemoveRequest rm(args);

    QSignalSpy response2(&rm, SIGNAL(finished()));
    QSignalSpy error2(&rm, SIGNAL(error(QtJsonDb::QJsonDbRequest::ErrorCode,QString)));

    db->send(&rm);
    waiting = true;
    do {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
        if (response2.count() || error2.count() || discon.count()) {
            waiting = false;
        }
    } while (waiting);
}

#endif
