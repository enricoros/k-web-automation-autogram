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

#ifndef __WCGame_h__
#define __WCGame_h__

#include "AbstractGame.h"
#include <QTime>
class Scrambler;

class WCGame : public AbstractGame
{
    public:
        WCGame( QObject * parent );
        ~WCGame();

        // ::AbstractGame
        QPixmap highlightPixmap( const QPixmap & pixmap ) const;
        void train( Ocr * ocr, QString lettersText, const QImage & gamePixmap ) const;
        void run( Ui::AppWidgetClass * ui, const ScreenCapture * capture, const Ocr * ocr );

    Q_SIGNALS:
        void gameEnded();

    private:
        Scrambler * m_wordsDictionary;
        Scrambler * m_namesDictionary;
        QTime m_time;
};

#endif
