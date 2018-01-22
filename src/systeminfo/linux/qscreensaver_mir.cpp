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

#include "qscreensaver_mir_p.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingCall>

QT_BEGIN_NAMESPACE

QScreenSaverPrivate::QScreenSaverPrivate(QScreenSaver *parent)
    : q_ptr(parent)
    , m_keepDisplayOnRequestId(-1)
    , m_iface(QLatin1String("com.canonical.Unity.Screen"),
              QLatin1String("/com/canonical/Unity/Screen"),
              QLatin1String("com.canonical.Unity.Screen"),
              QDBusConnection::systemBus())
{
}

bool QScreenSaverPrivate::screenSaverEnabled()
{
    return m_keepDisplayOnRequestId == -1;
}

void QScreenSaverPrivate::setScreenSaverEnabled(bool enabled)
{
    if (!m_iface.isValid())
        return;

    if (m_keepDisplayOnRequestId == -1 && !enabled) {
        // set request
        QDBusMessage reply = m_iface.call(QLatin1String("keepDisplayOn"));
        if (reply.arguments().count() > 0)
            m_keepDisplayOnRequestId = reply.arguments().first().toInt();
    } else if (m_keepDisplayOnRequestId != -1 && enabled) {
        // clear request
        m_iface.asyncCall(QLatin1String("removeDisplayOnRequest"), m_keepDisplayOnRequestId);
        m_keepDisplayOnRequestId = -1;
    }
}

QT_END_NAMESPACE

