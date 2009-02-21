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
#include <QTimer>

GameState::GameState( Capture * capture, QObject * parent )
    : QObject( parent )
    , m_capture( capture )
{
    qWarning( "<GAME>" );
    QTimer::singleShot( 3000, this, SLOT(slotTemp()) );
}

GameState::~GameState()
{
    // don't delete m_capture, since it's external
    qWarning( "</GAME>" );
}

void GameState::slotTemp()
{
    qWarning( "<GAME> ended" );
    emit gameEnded();
}
