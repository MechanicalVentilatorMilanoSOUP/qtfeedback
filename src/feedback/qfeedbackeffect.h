/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtFeedback module of the Qt Toolkit.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QFEEDBACKEFFECT_H
#define QFEEDBACKEFFECT_H

#include "qfeedbackglobal.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

class QFeedbackActuator;
class QFeedbackFileEffectPrivate;
class QFeedbackHapticsEffectPrivate;

class Q_FEEDBACK_EXPORT QFeedbackEffect : public QObject
{
    Q_OBJECT
    Q_ENUMS(Effect)
    Q_ENUMS(Duration)
    Q_ENUMS(State)
    Q_ENUMS(ErrorType)

    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

public:
    // Make sure these are kept up to date with the declarative version
    enum Effect {
        Undefined = -1, Press, Release, PressWeak, ReleaseWeak, PressStrong, ReleaseStrong, PressAndHold,
        DragStart, DragDropInZone, DragDropOutOfZone, DragCrossBoundary, Popup, PopupClose, Move,
        NumberOfEffects,
        UserEffect = 65535
    };

    enum Duration {
        Infinite = -1
    };

    enum State {
        Stopped,
        Paused,
        Running,
        Loading
    };

    enum ErrorType {
        UnknownError,
        DeviceBusy
    };

    explicit QFeedbackEffect(QObject *parent = 0);

    virtual State state() const = 0;
    virtual int duration() const = 0;

    //for themes
    static bool supportsThemeEffect();
    static bool playThemeEffect(Effect effect);

public Q_SLOTS:
    void start();
    void stop();
    void pause();

protected:
    virtual void setState(State) = 0;

Q_SIGNALS:
    void error(QFeedbackEffect::ErrorType) const; //when an error occurs
    void stateChanged();

private:
    friend class QFeedbackInterface;
};

class Q_FEEDBACK_EXPORT QFeedbackHapticsEffect : public QFeedbackEffect
{
    Q_OBJECT

    Q_PROPERTY(int duration READ duration WRITE setDuration)
    Q_PROPERTY(qreal intensity READ intensity WRITE setIntensity)
    Q_PROPERTY(int attackTime READ attackTime WRITE setAttackTime)
    Q_PROPERTY(qreal attackIntensity READ attackIntensity WRITE setAttackIntensity)
    Q_PROPERTY(int fadeTime READ fadeTime WRITE setFadeTime)
    Q_PROPERTY(qreal fadeIntensity READ fadeIntensity WRITE setFadeIntensity)
    Q_PROPERTY(int period READ period WRITE setPeriod)
    Q_PROPERTY(QFeedbackActuator* actuator READ actuator WRITE setActuator)

public:
    explicit QFeedbackHapticsEffect(QObject *parent = 0);
    ~QFeedbackHapticsEffect();

    void setDuration(int msecs);
    int duration() const;

    void setIntensity(qreal intensity);
    qreal intensity() const;

    //the envelope
    void setAttackTime(int msecs);
    int attackTime() const;

    void setAttackIntensity(qreal intensity);
    qreal attackIntensity() const;

    void setFadeTime(int msecs);
    int fadeTime() const;

    void setFadeIntensity(qreal intensity);
    qreal fadeIntensity() const;

    void setPeriod(int msecs);
    int period() const;

    void setActuator(QFeedbackActuator *actuator);
    QFeedbackActuator* actuator() const;

    //reimplementations from QFeedbackEffect
    virtual State state() const;

protected:
    virtual void setState(State);

private:
    Q_DISABLE_COPY(QFeedbackHapticsEffect)
    friend class QFeedbackHapticsEffectPrivate;
    QScopedPointer<QFeedbackHapticsEffectPrivate> priv;
};

class Q_FEEDBACK_EXPORT QFeedbackFileEffect : public QFeedbackEffect
{
    Q_OBJECT

    Q_PROPERTY(bool loaded READ isLoaded WRITE setLoaded)
    Q_PROPERTY(QUrl source READ source WRITE setSource)

public:
    explicit QFeedbackFileEffect(QObject *parent = 0);
    ~QFeedbackFileEffect();

    int duration() const;

    bool isLoaded() const;

    void load();
    void unload();
    void setLoaded(bool);

    QUrl source() const;
    void setSource(const QUrl &);

    static QStringList supportedMimeTypes();

    //reimplementations from QFeedbackEffect
    virtual State state() const;

protected:
    virtual void setState(State);

private:
    Q_DISABLE_COPY(QFeedbackFileEffect)
    friend class QFeedbackFileEffectPrivate;
    QScopedPointer<QFeedbackFileEffectPrivate> priv;
};

QT_END_NAMESPACE
QT_END_HEADER

#endif // QFEEDBACKEFFECT_H
