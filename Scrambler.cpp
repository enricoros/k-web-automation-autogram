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

#include "Scrambler.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

#define notImplemented() qDebug("%s %s not implemented", __FILE__, __FUNCTION__);

QStringList permute( const QString & str, int l );

Scrambler::Scrambler()
{
}

Scrambler::~Scrambler()
{
}

void Scrambler::addDictionary( const QString & filename)
{
    QFile f( filename );
    if( !f.open( QIODevice::ReadOnly ) ) {
        qWarning( "Scrambler::addDictionary can not open %s", qPrintable(filename) );
    }

    QTextStream s( &f );
    while( !s.atEnd() ) {
        QString w = s.readLine();
        QChar c = w.at( 0 );
        QStringList & sl = m_dictionaries[ c ];
        sl.append( w );
    }
}

QStringList Scrambler::words( const QString & base, int min, int max )
{
    QStringList res;
    QStringList all = allWords( base, min, max );

    foreach( const QString & s, all ) {
        if ( m_dictionaries.value( s.at( 0 ) ).contains( s ) ) {
            if ( !res.contains( s ) )
                res.append( s );
        }
    }
    return res;
}

QStringList Scrambler::allWords( const QString & base, int min, int max )
{
    min = qMax( 1, min );
    min = qMin( min, base.size() );
    max = qMax( min, max );

    QStringList r;
    for ( int i = min; i <= max; ++i )
        r.append( permute( base, i ) );

    return r;
}

void Scrambler::setOk( const QString & word )
{
    notImplemented();
}

void Scrambler::setFail( const QString & word )
{
    notImplemented();
}




QStringList permute( const QString & str, int l )
{
    QStringList r;

    if ( l == 1 ) {
        for( int i = 0; i < str.size() ; ++i ) {
            r.append( QString(str.at( i )) );
        }
        return r;
    }

    for( int i = 0; i < str.size() ; ++i ) {
        QString sub = str;
        sub.replace(i,1,QString("") );
        QStringList r1 = permute( sub, l-1 );
        foreach( const QString &s, r1 ) {
            QString a = s;
            a.prepend( str.at(i) );
            r.append( a );
        }
    }

    return r;
}
