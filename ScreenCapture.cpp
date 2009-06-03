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

#include "ScreenCapture.h"
#include <QCoreApplication>
#include <QDirIterator>
#include <QImage>
#include <QTimerEvent>
#ifdef Q_WS_X11
#include <QX11Info>
#else
#include <QApplication>
#endif
#include <QDesktopWidget>
#include <QPixmap>

ScreenCapture::ScreenCapture( QObject * parent )
    : QObject( parent )
    , m_enabled( false )
    , m_fps( 0 )
{
}

void ScreenCapture::setEnabled( bool enabled )
{
    m_enabled = enabled;
}

bool ScreenCapture::enabled() const
{
    return m_enabled;
}

void ScreenCapture::setGeometry( const QRect & geometry )
{
    m_geometry = geometry;
}

QRect ScreenCapture::geometry() const
{
    return m_geometry;
}

void ScreenCapture::setFrequency( int fps )
{
    m_fps = fps;
    m_timer.start( 100 * 1000 / m_fps, this );
}

int ScreenCapture::frequency() const
{
    return m_fps;
}

QPixmap ScreenCapture::lastPixmap() const
{
    return m_pixmap;
}

void ScreenCapture::timerEvent( QTimerEvent * event )
{
    if ( event->timerId() != m_timer.timerId() || m_geometry.isNull() )
        return QObject::timerEvent( event );

    if ( !m_enabled )
        return;

    m_pixmap = QPixmap::grabWindow(
#if defined(Q_WS_X11)
            QX11Info::appRootWindow(),
#else
            QApplication::desktop()->winId(),
#endif
            m_geometry.left(), m_geometry.top(), m_geometry.width(), m_geometry.height() );

    emit gotPixmap( m_pixmap, QCursor::pos() - QPoint( m_geometry.topLeft() ) );
}
