/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "dberror_p.h"

QT_BEGIN_NAMESPACE

DBError::DBError()
{
    setError(NoError);
}

void DBError::setError(ErrorCode error, const QString &text)
{
    m_error = error;
    switch (error) {
        case (NoError):
            m_text = QLatin1String("No error");
            break;
        case(DatabaseNotOpen):
            m_text = QLatin1String("Database not open");
            break;
        case(InvalidDatabaseConnection):
            m_text = QLatin1String("Invalid database connection");
            break;
        case(ExternalIfaceIDFound):
            m_text = QLatin1String("External InterfaceID found");
            break;
        case(SqlError):
        case(NotFound):
        case(LocationAlreadyRegistered):
        case(IfaceImplAlreadyRegistered):
        case(CannotCreateDbDir):
        case(InvalidDescriptorScope):
        case(IfaceIDNotExternal):
        case(InvalidDatabaseFile):
        case(NoWritePermissions):
        case(CannotOpenServiceDb):
            m_text = text;
            break;
        default:
            m_text= QLatin1String("Unknown error");
            m_error = UnknownError;
    }
}
QT_END_NAMESPACE
