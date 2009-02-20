#ifndef SCRAMBLER_H
#define SCRAMBLER_H

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
