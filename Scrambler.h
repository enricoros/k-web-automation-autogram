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

#ifndef __Scrambler_h__
#define __Scrambler_h__

#include <QStringList>
#include <QMap>

class Scrambler
{
    public:
        Scrambler();
        ~Scrambler();

        // 1 word 1 line
        void addDictionary( const QString & filename);

        // return all the word matching in the dictionary
        QStringList words( const QString & base, int min = 1, int max = -1 );

        // return
        QStringList allWords( const QString & base, int min = 1, int max = -1 );

        // feedback functions
        void setOk( const QString & word );
        void setFail( const QString & word );

    private:
        QMap<QChar, QStringList> m_dictionaries;
};

#endif // SCRAMBLER_H
