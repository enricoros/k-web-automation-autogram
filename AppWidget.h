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

#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>
class QPixmap;
class QPoint;
class QSettings;
class ScreenCapture;
class AbstractGame;
class Ocr;
class RegionWidget;

namespace Ui { class AppWidgetClass; }

class AppWidget : public QWidget
{
    Q_OBJECT
    public:
        AppWidget(QWidget *parent = 0);
        ~AppWidget();

    protected:
        void mousePressEvent(QMouseEvent * event);
        void mouseMoveEvent(QMouseEvent * event);
        void mouseReleaseEvent(QMouseEvent * event);

    private:
        void saveSettings();
        Ui::AppWidgetClass * ui;
        QSettings * m_settings;
        AbstractGame * m_game;
        ScreenCapture * m_capture;
        Ocr * m_ocr;
        RegionWidget * m_pickingRegion;

    private Q_SLOTS:
        void on_gameNo_toggled(bool checked);
        void on_gameWc_toggled(bool checked);
        void on_gameWcLearn_toggled(bool checked);
        void on_pickRegionButton_clicked();
        void on_trainButton_clicked();
        void slotOnTopChanged();
        void slotCapParamsChanged();
        void slotProcessPixmap( const QPixmap & pixmap, const QPoint & cursor );
};

#endif // APPWIDGET_H
