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

#ifndef INPUTUTILS_H
#define INPUTUTILS_H

#include <QObject>
#include <QPoint>

class InputUtils : public QObject
{
    public:
        // MOUSE
        static void mouseLeftClick();
        //static void mouseRightClick();
        static void mouseLeftPress();
        static void mouseLeftRelease();
        static void mouseMove( int x, int y );
        static inline void mouseMove( const QPoint & point ) { mouseMove( point.x(), point.y() ); }

        // KEYBOARD
        static void keyWrite( const QString & string );
        //static void keyClickQt( quint32 );
        static void keyClickKeysym( char latin1 );
        //static void keyPress( int qtKeycode );
        //static void keyRelease( int qtKeycode );
};

#endif // INPUTUTILS_H
