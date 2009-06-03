/***************************************************************************
 * Copyright (c) 2009 Enrico Ros                                           *
 *         2009 Enrico Ros <enrico.ros@email.it>                           *
 *         2009 Alberto Scarpa <skaal.sl@gmail.com>                        *
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
#elif defined(Q_WS_MAC)
#import <ApplicationServices/ApplicationServices.h>
#else
#warning InputUtils not implemented for this Windowing System
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
    XFlush( QX11Info::display() );
#elif defined(Q_WS_WIN)
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput( 1, &Input, sizeof(INPUT) );
#elif defined(Q_WS_MAC)
    QPoint currPos = QCursor::pos();
    // The data structure CGPoint represents a point in a two-dimensional
    // coordinate system.  Here, X and Y distance from upper left, in pixels.
    //
    CGPoint pt;
    pt.x = currPos.x();
    pt.y = currPos.y();
    // CGPostMouseEvent( CGPoint        mouseCursorPosition,
    //                   boolean_t      updateMouseCursorPosition,
    //                   CGButtonCount  buttonCount,
    //                   boolean_t      mouseButtonDown, ... )
    // So, we feed coordinates to CGPostMouseEvent, put the mouse there,
    // then click and release.
    CGPostMouseEvent( pt, 1, 1, 1 );
#endif
}

void InputUtils::mouseLeftRelease()
{
#if defined(Q_WS_X11)
    XTestFakeButtonEvent( QX11Info::display(), 1, false, CurrentTime );
    XFlush( QX11Info::display() );
#elif defined(Q_WS_WIN)
    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput( 1, &Input, sizeof(INPUT) );
#elif defined(Q_WS_MAC)
    CGPoint pt;
    QPoint currPos = QCursor::pos();
    pt.x = currPos.x();
    pt.y = currPos.y();
    CGPostMouseEvent( pt, 1, 1, 0 );
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
        keyClick( string.at( i ) );
}

#if defined(Q_WS_X11)
static void xSendScanCode( Display * display, unsigned int keyCode )
{
    XTestFakeKeyEvent( display, keyCode, true, CurrentTime );
    XTestFakeKeyEvent( display, keyCode, false, CurrentTime );
    XFlush( display );
}
#endif

void InputUtils::keyClick( const QChar & singleChar )
{
#if defined(Q_WS_X11)
    char latin1 = singleChar.toLatin1();
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
    xSendScanCode( display, keyCode );
#elif defined(Q_WS_WIN)
    // keypress
    SHORT virtKey = VkKeyScan(singleChar.unicode());
    INPUT input[2];
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = LOBYTE(virtKey);
    input[0].ki.wScan = 0;
    input[0].ki.dwFlags = 0;
    input[0].ki.time = 0;
    input[0].ki.dwExtraInfo = 0;
    // keyrelease
    input[1] = input[0];
    input[1].ki.dwFlags |= KEYEVENTF_KEYUP;
    // send both
    ::SendInput( 2, input, sizeof(INPUT) );
#else
    qWarning( "InputUtils::keyClick: notImplemented()");
#endif
}

void InputUtils::keyClickSpecial( int qtKeyCode )
{
#if defined(Q_WS_X11)
    Display * display = QX11Info::display();
#elif defined(Q_WS_WIN)
    // pre-init Virtual Input structure
    INPUT vinput[2];
    vinput[0].type = INPUT_KEYBOARD;
    vinput[0].ki.wVk = 0;
    vinput[0].ki.wScan = 0;
    vinput[0].ki.dwFlags = 0;
    vinput[0].ki.time = 0;
    vinput[0].ki.dwExtraInfo = 0;
#endif

    switch ( qtKeyCode ) {
        case Qt::Key_Control:
#if defined(Q_WS_X11)
            xSendScanCode( display, XKeysymToKeycode( display, KS_X11_CTRL_L ) );
#elif defined(Q_WS_WIN)
            vinput[0].ki.wVk = VK_CONTROL;
#else
            qWarning("InputUtils::keyClickSpecial: CTRL not implemented for this platform");
#endif
            break;

        default:
            qWarning( "InputUtils::keyClickSpecial: cannot get keycode of special qtkey '%d', please provide the conversion.", qtKeyCode );
            break;
    }

#if defined(Q_WS_WIN)
    // send keypress and keyrelease
    if ( vinput[0].ki.wVk ) {
        vinput[1] = vinput[0];
        vinput[1].ki.dwFlags |= KEYEVENTF_KEYUP;
        ::SendInput( 2, vinput, sizeof(INPUT) );
    }
#endif
}
