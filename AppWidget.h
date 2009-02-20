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

#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>
class QPixmap;
class QPoint;
class QSettings;
class Capture;
class Scrambler;

namespace Ui { class AppWidgetClass; }

class AppWidget : public QWidget
{
    Q_OBJECT
    public:
        AppWidget(QWidget *parent = 0);
        ~AppWidget();

    private:
        void saveSettings();
        Ui::AppWidgetClass * ui;
        QSettings * m_settings;
        Capture * m_capture;
        Scrambler * m_scrambler;

    private Q_SLOTS:
        void on_letters_textChanged( const QString & string );
        void slotOnTopChanged();
        void slotCapParamsChanged();
        void slotProcessPixmap( const QPixmap & pixmap, const QPoint & cursor );
};

#endif // APPWIDGET_H
