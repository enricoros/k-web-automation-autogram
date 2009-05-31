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

#ifndef __AbstractGame_h__
#define __AbstractGame_h__

#include <QObject>
#include <QPixmap>
class Ocr;
class ScreenCapture;
namespace Ui { class AppWidgetClass; }

class AbstractGame : public QObject
{
    Q_OBJECT
    public:
        AbstractGame( QObject * parent = 0 );
        virtual ~AbstractGame();

        // utility functions
        virtual QPixmap highlightPixmap( const QPixmap & pixmap ) const = 0;
        virtual void train( Ocr * ocr, QString lettersText, const QImage & gamePixmap ) const = 0;

        // game logic
        // ...
        virtual void run( Ui::AppWidgetClass * ui, const ScreenCapture * capture, const Ocr * ocr ) = 0;

    Q_SIGNALS:
        void gameEnded();
};

#endif // __AbstractGame_h__
