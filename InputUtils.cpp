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
//include <X11/keysymdef.h> defines:
#define KS_X11_BACKSPACE    0xff08
#define KS_X11_ESCAPE       0xff1b
#define KS_X11_RETURN       0xff0d
#define KS_X11_SHIFT_L      0xffe1
#define KS_X11_SHIFT_R      0xffe2
#define KS_X11_CTRL_L       0xffe3
#define KS_X11_CTRL_R       0xffe4
#define KS_X11_LEFT         0xff51
#define KS_X11_UP           0xff52
#define KS_X11_RIGHT        0xff53
#define KS_X11_DOWN         0xff54
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

void InputUtils::keyWrite( const QString & string )
{
    int len = string.length();
    for ( int i = 0; i < len; i++ )
        keyClickKeysym( string.at( i ).toLatin1() );
}

void InputUtils::keyClickKeysym( char latin1 )
{
#if defined(Q_WS_X11)
    Display * display = QX11Info::display();
    unsigned int keyCode = XKeysymToKeycode( display, latin1 );
    if ( !keyCode ) {
        if ( latin1 == '\n' )
            keyCode = XKeysymToKeycode( display, KS_X11_RETURN );
        if ( !keyCode ) {
            qWarning( "InputUtils::keyClickKeysym(X11): cannot get keycode of char '%d', please provide the conversion.", latin1 );
            return;
        }
    }
    XTestFakeKeyEvent( display, keyCode, true, CurrentTime );
    XTestFakeKeyEvent( display, keyCode, false, CurrentTime );
#else
    qWarning( "InputUtils::keyClick: notImplemented()");
#endif
}
