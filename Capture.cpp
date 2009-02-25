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

#include "Capture.h"
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

Capture::Capture( QObject * parent )
    : QObject( parent )
    , m_enabled( false )
    , m_fps( 0 )
{
}

void Capture::setEnabled( bool enabled )
{
    m_enabled = enabled;
}

bool Capture::enabled() const
{
    return m_enabled;
}

void Capture::setGeometry( const QRect & geometry )
{
    m_geometry = geometry;
}

QRect Capture::geometry() const
{
    return m_geometry;
}

void Capture::setFrequency( int fps )
{
    m_fps = fps;
    m_timer.start( 1000 / m_fps, this );
}

int Capture::frequency() const
{
    return m_fps;
}

QPixmap Capture::currentPixmap() const
{
    return m_pixmap;
}

void Capture::timerEvent( QTimerEvent * event )
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
