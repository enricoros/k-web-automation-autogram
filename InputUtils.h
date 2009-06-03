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

#ifndef __InputUtils_h__
#define __InputUtils_h__

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
        static void keyClick( const QChar & singleChar );
        static void keyClickSpecial( int qtKeyCode );
        //static void keyPress( int qtKeycode );
        //static void keyRelease( int qtKeycode );
};

#endif // INPUTUTILS_H
