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

#include "Ocr.h"
#include <QFontMetrics>
#include <QPainter>
#include <QBitmap>
#include <QTransform>
#include <QPixmap>
#include <math.h>

#include <QDebug>
#include <QLabel>

struct OcrGlyph {
    QChar character;
    QImage image;
    int width;
    int height;
    double ratio;
    // ADD more and more properties ...
};


// CORE FUNCTIONS
static QImage trimImage( const QImage & image )
{
    return image;
}

static OcrResult compareGlyph( const QImage & image, OcrGlyph * glyph )
{
    OcrResult result;
    result.character = glyph->character;
    result.confidence = 0.0;

    // bail out if ratios are too different
    double ratio = (float)image.width() / (float)image.height();
    double k2 = (ratio / glyph->ratio + glyph->ratio / ratio) / 2;
    if ( k2 > 1.05 )
        return result;

    // ALGO
    // scale glyph image size to image size
#if 0
    QTransform t;
    t.scale( (float)glyph->image.width() / (float)image.width(), (float)glyph->image.height() / (float)image.height() );
    QImage glyphImage = glyph->image.transformed( t.inverted(0), Qt::SmoothTransformation );
#endif
    QImage glyphImage = glyph->image.scaled( image.width(), image.height() );

    int width = glyphImage.width();
    int height = glyphImage.height();
    int pixels = width * height;
    unsigned int error = 0;
    unsigned int analyzed = 0;
    for ( int y = 0; y < height; y++ ) {
        for ( int x = 0; x < width; x++ ) {
            int v1 = qGray( image.pixel( x, y ) );
            int v2 = qGray( glyphImage.pixel( x, y ) );
            if ( v1 >= 254 && v2 >= 254 )
                continue;
            analyzed++;
            int diff = (v1 > v2 ? v1 - v2 : v2 - v1);
            error += (diff * diff) >> 8;
        }
    }
    if ( !analyzed )
        return result;
    float KX = sqrt( (float)error / (254.0 * (float)analyzed) );

    if ( glyph->character == '0' || glyph->character == 'O' ) {
        QLabel * label = new QLabel();
        label->setPixmap( QPixmap::fromImage( image ) );
        label->show();

        label = new QLabel();
        label->setPixmap( QPixmap::fromImage( glyphImage ) );
        label->show();
    }

    result.confidence = 1.0 - KX;
    qWarning() << result.confidence << result.character << pixels << analyzed;
    return result;
}

Ocr::Ocr()
{
}

Ocr::~Ocr()
{
    clearTraning();
}

void Ocr::trainFont( const QFont & font, QFontDatabase::WritingSystem writingSystem )
{
    // checks
    if ( writingSystem != QFontDatabase::Latin ) {
        qWarning( "Ocr::trainFont: unsupported writing system" );
        return;
    }

    // add chars
    QList<QChar> chars;
    // symbols !"#$%&'()*+,-./
    //for ( int i = 33; i <= 47; i++ )
    //    chars.append( QChar( i ) );
    // numbers
    for ( int i = 48; i <= 57; i++ )
        chars.append( QChar( i ) );
    // symbols :;<=>?@
    //for ( int i = 58; i <= 64; i++ )
    //    chars.append( QChar( i ) );
    // uppercase letters
    for ( int i = 65; i <= 90; i++ )
        chars.append( QChar( i ) );
    // symbols [\]^_`
    //for ( int i = 91; i <= 96; i++ )
    //    chars.append( QChar( i ) );
    // lowercase letters
    for ( int i = 97; i <= 122; i++ )
        chars.append( QChar( i ) );
    // symbols {|}~
    //for ( int i = 123; i <= 126; i++ )
    //    chars.append( QChar( i ) );

    // generate glyphs for Latin1 and train the system
    QFontMetrics metrics( font );
    foreach ( const QChar & character, chars ) {
        QRect rect = metrics.boundingRect( character ).adjusted( -5, -5, 5, 5 );
        if ( !rect.isValid() ) {
            qWarning( "Ocr::trainFont: invalid rect for font" );
            continue;
        }

        // draw glyph
        QImage image( rect.width(), rect.height(), QImage::Format_ARGB32 );
        image.fill( 0xFFFFFFFF );
        QPainter imgPainter( &image );
        imgPainter.setFont( font );
        imgPainter.setPen( Qt::black );
        imgPainter.drawText( 0, 0, rect.width(), rect.height(), Qt::AlignCenter, character );
        imgPainter.end();

        // recognize glyph
        trainGlyph( image, character );
    }
}

void Ocr::trainGlyph( const QImage & sourceImage, const QChar & character )
{
#if 0
    QLabel * label = new QLabel();
    label->setPixmap( QPixmap::fromImage( sourceImage ) );
    label->setWindowTitle( character );
    label->show();
#endif

    // checks
    if ( sourceImage.isNull() || sourceImage.width() < 4 || sourceImage.height() < 4 || character.isNull() ) {
        qWarning( "Ocr::trainGlyph: invalid input" );
        return;
    }

    // prepare image for processing
    QImage image = trimImage( sourceImage );

    // append glyph description
    OcrGlyph * glyph = new OcrGlyph();
    glyph->character = character;
    glyph->image = image;
    glyph->width = image.width();
    glyph->height = image.height();
    glyph->ratio = (float)image.width() / (float)image.height();
    m_glyphs.append( glyph );
}

void Ocr::clearTraning()
{
    qDeleteAll( m_glyphs );
    m_glyphs.clear();
}

OcrResult Ocr::recognizeGlyph( const QImage & sourceImage, const QRect & __rect )
{
    // checks
    OcrResult result;
    result.character = '?'; // FIXME: REMOVE THIS
    result.confidence = 0.0;
    if ( sourceImage.isNull() )
        return result;

    // prepare image for processing
    QRect rect = __rect.isValid() ? __rect : QRect( 0, 0, sourceImage.width(), sourceImage.height() );
    QImage image = trimImage( sourceImage.copy( rect ) );

    // match all glyphs
    foreach ( OcrGlyph * glyph, m_glyphs ) {
        OcrResult comparison = compareGlyph( image, glyph );
        if ( comparison.confidence > result.confidence ) {
            result.character = comparison.character;
            result.confidence = comparison.confidence;
        }
    }

    // done!
    return result;
}
