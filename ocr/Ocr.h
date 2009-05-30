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

#ifndef __Ocr_h__
#define __Ocr_h__

#include <QObject>
#include <QFont>
#include <QImage>
#include <QRect>
#include <QList>
#include <QChar>
#include <QFontDatabase>
class OcrGlyph;

struct OcrResult {
    QChar character;
    double confidence;
};

class Ocr
{
    public:
        Ocr();
        ~Ocr();

        // train the OCR
        void trainFont( const QFont & font, QFontDatabase::WritingSystem writingSystem = QFontDatabase::Latin );
        void trainGlyph( const QImage & image, const QChar & character );
        void clearTraning();

        // recognize a glyph
        OcrResult recognizeGlyph( const QImage & image, const QRect & rect = QRect() );

    private:
        QList< OcrGlyph * > m_glyphs;
};

#endif
