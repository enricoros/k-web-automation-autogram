/***************************************************************************
 * Copyright (c) 2009 FAGAS                                                *
 *         2009 Koral <koral@email.it>                                     *
 *         2009 Skaal <skaal.sl@gmail.com>                                 *
 *                                                                         *
 * Permission is hereby granted, free of charge, to any person             *
 * obtaining a copy of this software and associated documentation          *
 * files (the "Software"), to deal in the Software without                 *
 * restriction, including without limitation the rights to use,            *
 * copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the               *
 * Software is furnished to do so, subject to the following                *
 * conditions:                                                             *
 *                                                                         *
 * The above copyright notice and this permission notice shall be          *
 * included in all copies or substantial portions of the Software.         *
 *                                                                         *
 ***************************************************************************/

#include "InputUtils.h"
#include <QPoint>
#include <QCursor>

#if defined(Q_WS_X11)
#include <QX11Info>
#include <X11/extensions/XTest.h>
#elif defined(Q_WS_WIN)
#include <windows.h>
#else
#warning leftClick not implemented for this Windowing System
#endif

void InputUtils::mouseLeftClick()
{
    mouseLeftPress();
    mouseLeftRelease();
}

void InputUtils::mouseLeftPress()
{
#if defined(Q_WS_X11)
    XTestFakeButtonEvent( QX11Info::display(), 1, true, CurrentTime );
#elif defined(Q_WS_WIN)
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput( 1, &Input, sizeof(INPUT) );
#endif
}

void InputUtils::mouseLeftRelease()
{
#if defined(Q_WS_X11)
    XTestFakeButtonEvent( QX11Info::display(), 1, false, CurrentTime );
#elif defined(Q_WS_WIN)
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput( 1, &Input, sizeof(INPUT) );
#endif
}

void InputUtils::mouseMove( int x, int y )
{
    QCursor::setPos( x, y );
}

void InputUtils::keyWrite( const QString & /*string*/ )
{
    qWarning( "InputUtils::keyWrite: notImplemented()");
}

void InputUtils::keyClick( int /*qtKeyCode*/ )
{
    qWarning( "InputUtils::keyClick: notImplemented()");
}
