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

#ifndef __Capture_h__
#define __Capture_h__

#include <QObject>
#include <QBasicTimer>
#include <QList>
#include <QImage>

class Capture : public QObject
{
    Q_OBJECT
    public:
        Capture( QObject * parent );

        void setEnabled( bool enabled );
        bool enabled() const;
        void setGeometry( const QRect & geometry );
        QRect geometry() const;
        void setFrequency( int fps );
        int frequency() const;

    Q_SIGNALS:
        void gotPixmap( const QPixmap & pixmap, const QPoint & cursorPos );

    protected:
        void timerEvent( QTimerEvent * event );

    private:
        QBasicTimer m_timer;
        QRect m_geometry;
        bool m_enabled;
        int m_fps;
};

#endif
