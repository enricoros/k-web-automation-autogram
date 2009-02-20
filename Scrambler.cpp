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
#include <QDebug>

#include<iostream>
using namespace std;

#define notImplemented() qDebug("%s %s not implemented", __FILE__, __FUNCTION__);

long long totCount ;


void circularPrint(char *str)
{

    int  len = strlen(str);
    char *mystr = new char [len +1];
    int i,j;

    for(i=0; i <len; i++ ) {
        for(j=0;j<len; j++ )
            mystr[j] = str[(i+j)%len];
        mystr[j] =0;
        totCount++;
        // comment the line below to supress the string output
        cout << mystr  << endl;

    }

    delete []mystr;
    return ;
}


void permutation(char prefix[],char suffix[])
{
  int length=strlen(suffix);
  int len = strlen(prefix) ;
  int i=0, j=0;
  char *myprefix = new char [len];

  if(len ) {
    strncpy(myprefix, prefix, len - 1 );
    myprefix[len-1]= 0;

    for(i=0; i< length ; i++) {

        char *mysuffix = new char [length+2];
        mysuffix[0] = prefix[len-1];

        // rotate the current append and prepare append for next
        for(j=0; j< length ; j++ ) {
            mysuffix[j+1] = suffix[(i+j)%length];
        }

        mysuffix [j+1]= 0;
        permutation(myprefix, mysuffix);
        delete []mysuffix;
    }
  }
  else {
    // found permutation, now find other
    // permutations by rotating the string
    circularPrint(suffix);
  }
  delete []myprefix;
}







Scrambler::Scrambler()
{
}

Scrambler::~Scrambler()
{
}

void Scrambler::addDictionary( const QString & filename)
{
    notImplemented();
}

QStringList Scrambler::words( const QString & base, int min, int max )
{
    notImplemented();
    return QStringList();
}

QStringList Scrambler::allWords( const QString & base, int min, int max )
{
    notImplemented();
    char *c = "";
    char *c1 = "adf";
    permutation( c, c1);
    return QStringList();
}

void Scrambler::setOk( const QString & word )
{
    notImplemented();
}

void Scrambler::setFail( const QString & word )
{
    notImplemented();
}