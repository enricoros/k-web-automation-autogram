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

#include "WCGame.h"

#include "Scrambler.h"
#include "ScreenCapture.h"
#include "InputUtils.h"
#include "ocr/Ocr.h"
#include "ui_AppWidget.h"
#include <QTimer>
#include <QDebug>
#include <QPainter>

//BEGIN portableMSleep
#ifdef Q_OS_UNIX
#include <unistd.h>
void portableMSleep( int ms ) {
    ::usleep( ms * 1000 );
}
#else
#ifdef Q_OS_WIN32
#include <windows.h>
void portableMSleep( int ms ) {
    Sleep( ms );
}
#endif
#endif
//END portableMSleep


//static const int WC_CAP_L = 114;
//static const int WC_CAP_T = 245;
static const int WC_BASELINE_H[] = { 7, 61, 116, 170, 224, 279 };
static const int WC_BASELINE_V[] = { 3,  3,   3,   3,   3,   3 };
static const int WC_LETTER_W = 35;
static const int WC_LETTER_H = 37;

WCGame::WCGame( QObject * parent )
    : AbstractGame( parent )
    , m_wordsDictionary( new Scrambler() )
    , m_namesDictionary( new Scrambler() )
{
    // load the dictionaries
    m_wordsDictionary->addDictionary( "dic-it.txt" );
    m_namesDictionary->addDictionary( "dic-enrico.txt" );
}

WCGame::~WCGame()
{
    delete m_wordsDictionary;
    delete m_namesDictionary;
}

QPixmap WCGame::highlightPixmap( const QPixmap & pixmap ) const
{
    QPixmap pix = pixmap;
    QPainter pp( &pix );
    pp.setPen( Qt::red );
    for ( int i = 0; i < 6; i++ )
        pp.drawRect( WC_BASELINE_H[ i ], WC_BASELINE_V[ i ], WC_LETTER_W - 1, WC_LETTER_H - 1 );
    pp.end();
    return pix;
}

void WCGame::train( Ocr * ocr, QString lettersText, const QImage & gamePixmap ) const
{
    for ( int i = 0; i < 6; i++ ) {
        QImage letterImage = gamePixmap.copy( WC_BASELINE_H[ i ], WC_BASELINE_V[ i ], WC_LETTER_W, WC_LETTER_H );
        QChar character = lettersText.at( i );
        letterImage.save( QString( "wc-glyphs/glyph_%1.png" ).arg( character.toLatin1() ), "PNG" );
        ocr->trainGlyph( letterImage, character );
    }
}

void WCGame::run( Ui::AppWidgetClass * ui, const ScreenCapture * capture, const Ocr * ocr )
{
    if ( m_time.isNull() )
        m_time.start();
    if ( m_time.elapsed() > (60 * 60 * 1000) )
        return;

    // OCR screen -> 6Chars + mean color identification
    QString letters;
    QImage gamePixmap = capture->lastPixmap().toImage();
    int meanR = 0, meanG = 0, meanB = 0;
    for ( int i = 0; i < 6; i++ ) {
        // accumulate intermediate color (yellow for normal, pink for name)
        if ( i > 1 ) {
            int sampleX = (WC_BASELINE_H[ i ] + WC_BASELINE_H[ i - 1 ] + WC_LETTER_W) >> 1;
            int sampleY = (WC_BASELINE_V[ i ] + WC_LETTER_H) >> 1;
            QRgb sampleColor = gamePixmap.pixel( sampleX, sampleY );
            meanR += qRed( sampleColor );
            meanG += qGreen( sampleColor );
            meanB += qBlue( sampleColor );
        }
        QImage letterImage = gamePixmap.copy( WC_BASELINE_H[ i ], WC_BASELINE_V[ i ], WC_LETTER_W, WC_LETTER_H );
        OcrResult res = ocr->recognizeGlyph( letterImage );
        if ( res.confidence > 0.1 )
            letters.append( res.character.toLower() );
        //qWarning() << res.character << res.confidence;
    }
    ui->currentLetters->setText( letters );
    if ( letters.length() != 6 )
        return;

    // select the dictionary based on the game type
    qreal meanHue = QColor( meanR / 5, meanG / 5, meanB / 5 ).hueF();
    Scrambler * scrambler = (meanHue > 0.89 && meanHue < 0.99) ? m_namesDictionary : m_wordsDictionary;

    // Do Anagram
    QStringList words = scrambler->words( letters, ui->minLetters->value(), ui->maxLetters->value() );
    ui->anagramWords->setText(QString::number(words.size()));

    // Write Words
    InputUtils::mouseMove( capture->geometry().center() );
    InputUtils::mouseLeftClick();
    portableMSleep( 200 );
    int count = words.size();
    for ( int i = 0; i < count; i++ ) {
        InputUtils::keyWrite( words.at( i ) + '\n' );
        portableMSleep( 10 );
    }
    InputUtils::keyClickSpecial( Qt::Key_Control );
}
