/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd and/or its subsidiary(-ies).
** Copyright (C) 2018 BlackBerry Limited. All rights reserved.
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

#include "qsysteminfodata_simulator_p.h"
#include <QtCore/QDataStream>

QT_BEGIN_NAMESPACE

void qt_registerSystemInfoTypes()
{
    qRegisterMetaTypeStreamOperators<QBatteryInfoData>("QBatteryInfoData");
}

QDataStream &operator<<(QDataStream &out, const QBatteryInfoData &s)
{
    out << static_cast<qint32>(s.chargingState) << static_cast<qint32>(s.chargerType)
        << static_cast<qint32>(s.levelStatus);

    out << static_cast<qint32>(s.health);
    out << s.temperature;

    out << static_cast<qint32>(s.currentFlow) << static_cast<qint32>(s.maximumCapacity)
        << static_cast<qint32>(s.remainingCapacity) << static_cast<qint32>(s.remainingChargingTime)
        << static_cast<qint32>(s.voltage);

    return out;
}

QDataStream &operator>>(QDataStream &in, QBatteryInfoData &s)
{
    qint32 chargingState, chargerType, batteryStatus;
    in >> chargingState >> chargerType >> batteryStatus;

    s.chargingState = static_cast<QBatteryInfo::ChargingState>(chargingState);
    s.chargerType = static_cast<QBatteryInfo::ChargerType>(chargerType);
    s.levelStatus = static_cast<QBatteryInfo::LevelStatus>(batteryStatus);

    qint32 health;
    in >> health;
    s.health = static_cast<QBatteryInfo::Health>(health);

    in >> s.temperature;

    qint32 currentFlow, maximumCapacity, remainingCapacity, remainingChargingTime, voltage;
    in >> currentFlow >> maximumCapacity >> remainingCapacity >> remainingChargingTime >> voltage;

    s.currentFlow = currentFlow;
    s.maximumCapacity = maximumCapacity;
    s.remainingCapacity = remainingCapacity;
    s.remainingChargingTime = remainingChargingTime;
    s.voltage = voltage;

    return in;
}

QT_END_NAMESPACE


