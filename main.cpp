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

#include <QtGui/QApplication>
#include "AppWidget.h"
#include "ocr/Ocr.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppWidget w;
    w.show();
    Ocr ocr;
    ocr.trainFont( QFont( "Arial", 22 ) );
    QImage aImg( "/root/a.png" );
    OcrResult res = ocr.recognizeGlyph( aImg );
    qWarning() << res.character << res.confidence;
    return a.exec();
}
