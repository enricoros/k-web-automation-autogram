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

#include "GameState.h"

#include "Scrambler.h"
#include "Capture.h"
#include "InputUtils.h"
#include "ocr/Ocr.h"
#include <QTimer>
#include <unistd.h>
#include <QDebug>
#include "ui_AppWidget.h"

GameState::GameState( Ui::AppWidgetClass * ui, Capture * capture, Ocr * ocr, QObject * parent )
    : QObject( parent )
    , m_ui( ui )
    , m_capture( capture )
    , m_ocr( ocr )
{
    // create the Scrambler
    m_scrambler = new Scrambler();
    m_scrambler->addDictionary( "dic-it.txt" );

    QTimer::singleShot( 100, this, SLOT(slotPlay()) );
}

GameState::~GameState()
{
    // don't delete m_capture, since it's external
    delete m_scrambler;
}

void GameState::slotPlay()
{
    if ( m_time.isNull() )
        m_time.start();
    if ( m_time.elapsed() > (60 * 60 * 1000) )
        return;

    // find out the string
    QString letters;
    QImage gamePixmap = m_capture->currentPixmap().toImage();
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
    m_ui->currentLetters->setText( letters );

    // TEMP FOR TESTING
    QStringList words;
    if ( m_ui->allWords->isChecked() )
        words = m_scrambler->allWords( letters, m_ui->minLetters->value(), m_ui->maxLetters->value() );
    else
        words = m_scrambler->words( letters, m_ui->minLetters->value(), m_ui->maxLetters->value() );
    m_ui->words->setPlainText( words.join( "," ) );

    InputUtils::mouseMove( m_capture->geometry().center() );
    InputUtils::mouseLeftClick();
    usleep( 200 * 1000 );
    int count = words.size();
    for ( int i = 0; i < count; i++ ) {
        InputUtils::keyWrite( words.at( i ) + '\n' );
        usleep( 20 * 1000 );
    }
    InputUtils::keyClickSpecial( Qt::Key_Control );

    QTimer::singleShot( 2000, this, SLOT(slotPlay()) );
}
