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

#include "GameState.h"

#include "Scrambler.h"
#include "Capture.h"
#include "InputUtils.h"
#include "ocr/Ocr.h"
#include <QTimer>
#include <unistd.h>
#include <QDebug>
#include <QLabel>

GameState::GameState( Capture * capture, const QString & /*letters*/, QObject * parent )
    : QObject( parent )
    , m_capture( capture )
    , m_ocr( new Ocr() )
{
    qWarning( "<GAME>" );

    // create the Scrambler
    m_scrambler = new Scrambler();
    m_scrambler->addDictionary( "dic-it.txt" );

    // init the Ocr
    QFont font( "Arial", 32 );
    font.setBold( true );
    m_ocr->trainFont( font );

    // find out the string
    QString letters;
    QImage gamePixmap = capture->currentPixmap().toImage();
    int pixLetters[ 6 ] = { 121, 175, 230, 284, 338, 393 };
    int pixLetterTop = 248;
    int pixLetterWidth = 35;
    int pixLetterHeight = 37;
    for ( int i = 0; i < 6; i++ ) {
        QImage letter = gamePixmap.copy( pixLetters[ i ], pixLetterTop, pixLetterWidth, pixLetterHeight );
        OcrResult res = m_ocr->recognizeGlyph( letter );
        if ( res.confidence > 0.1 )
            letters.append( res.character.toLower() );
        //qWarning() << res.character << res.confidence;
    }
    QLabel * lab = new QLabel( letters );
    lab->setFixedSize( 32, 32 );
    lab->show();

    // TEMP FOR TESTING
    QStringList words = m_scrambler->words( letters, 3, 5 );

    InputUtils::mouseMove( m_capture->geometry().center() );
    InputUtils::mouseLeftClick();
    usleep( 200 * 1000 );
    int count = words.size();
    for ( int i = 0; i < count; i++ ) {
        InputUtils::keyWrite( words.at( i ) + '\n' );
        usleep( 20 * 1000 );
    }
    InputUtils::keyClickSpecial( Qt::Key_Control );

    //QTimer::singleShot( 3000, this, SLOT(slotTemp()) );
}

GameState::~GameState()
{
    // don't delete m_capture, since it's external
    qWarning( "</GAME>" );

    delete m_ocr;
    delete m_scrambler;
}

void GameState::slotTemp()
{
    qWarning( "<GAME> ended" );
    emit gameEnded();
}
