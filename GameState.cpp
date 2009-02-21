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
#include <QTimer>
#include <unistd.h>
#include <QDebug>

GameState::GameState( Capture * capture, const QString & letters, QObject * parent )
    : QObject( parent )
    , m_capture( capture )
{
    qWarning( "<GAME>" );

    // create the Scrambler
    m_scrambler = new Scrambler();
    m_scrambler->addDictionary( "dic-it.txt" );

    // TEMP FOR TESTING
    QStringList words = m_scrambler->words( letters, 3, 6 );


    InputUtils::mouseMove( m_capture->geometry().center() );
    InputUtils::mouseLeftClick();
    usleep( 200 * 1000 );
    int count = words.size();
    for ( int i = 0; i < count; i++ ) {
        InputUtils::keyWrite( words.at( i ) + '\n' );
        usleep( 200 * 1000 );
    }
    InputUtils::keyClickSpecial( Qt::Key_Control );

    QTimer::singleShot( 3000, this, SLOT(slotTemp()) );
}

GameState::~GameState()
{
    // don't delete m_capture, since it's external
    qWarning( "</GAME>" );

    delete m_scrambler;
}

void GameState::slotTemp()
{
    qWarning( "<GAME> ended" );
    emit gameEnded();
}
