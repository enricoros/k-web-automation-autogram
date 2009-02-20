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

#include "AppWidget.h"
#include "ui_AppWidget.h"
#include "Capture.h"
#include "Scrambler.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QPoint>
#include <QSettings>
#include <QButtonGroup>

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

AppWidget::AppWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AppWidgetClass)
#if defined(Q_OS_WIN)
    , m_settings( new QSettings( "biocheat.ini", QSettings::IniFormat ) )
#else
    , m_settings( new QSettings() )
#endif
{
    // create ui
    ui->setupUi(this);
    QDesktopWidget dw;
    ui->left->setMaximum( dw.width() );
    ui->top->setMaximum( dw.height() );
    ui->width->setMaximum( dw.width() );
    ui->height->setMaximum( dw.height() );
    // init defaults..
    if ( !m_settings->contains( "rect/left" ) ) {
        ui->left->setValue( (dw.width() - DEFAULT_WIDTH) / 3 );
        ui->top->setValue( (dw.height() - DEFAULT_HEIGHT) / 3 );
        ui->width->setValue( DEFAULT_WIDTH );
        ui->height->setValue( DEFAULT_HEIGHT );
    }
    // ..or reload previous values
    else {
        ui->left->setValue( m_settings->value( "rect/left" ).toInt() );
        ui->top->setValue( m_settings->value( "rect/top" ).toInt() );
        ui->width->setValue( m_settings->value( "rect/width" ).toInt() );
        ui->height->setValue( m_settings->value( "rect/height" ).toInt() );
        ui->frequency->setValue( m_settings->value( "rect/frequency" ).toInt() );
        ui->onTop->setChecked( m_settings->value( "rect/ontop" ).toBool() );
    }
    connect( ui->left, SIGNAL(valueChanged(int)), this, SLOT(slotCapParamsChanged()) );
    connect( ui->top, SIGNAL(valueChanged(int)), this, SLOT(slotCapParamsChanged()) );
    connect( ui->width, SIGNAL(valueChanged(int)), this, SLOT(slotCapParamsChanged()) );
    connect( ui->height, SIGNAL(valueChanged(int)), this, SLOT(slotCapParamsChanged()) );
    connect( ui->frequency, SIGNAL(valueChanged(int)), this, SLOT(slotCapParamsChanged()) );
    connect( ui->onTop, SIGNAL(toggled(bool)), this, SLOT(slotOnTopChanged()) );
    QButtonGroup * buttGroup = new QButtonGroup( this );
    buttGroup->addButton( ui->btnOff );
    buttGroup->addButton( ui->btnGame );
    buttGroup->addButton( ui->btnChallenge );
    buttGroup->addButton( ui->btnLearn1 );
    buttGroup->addButton( ui->btnLearn2 );
    slotOnTopChanged();

    // create the Scrambler
    m_scrambler = new Scrambler();

    // create the capture
    m_capture = new Capture( this );
    connect( m_capture, SIGNAL(gotPixmap(const QPixmap &, const QPoint &)),
             this, SLOT(slotProcessPixmap(const QPixmap &, const QPoint &)) );
    slotCapParamsChanged();

    // ### TEMP
    m_capture->setEnabled( true );
}

AppWidget::~AppWidget()
{
    saveSettings();
    delete m_settings;
    delete m_scrambler;
    delete m_capture;
    delete ui;
}

void AppWidget::saveSettings()
{
    m_settings->setValue( "rect/left", ui->left->value() );
    m_settings->setValue( "rect/top", ui->top->value() );
    m_settings->setValue( "rect/width", ui->width->value() );
    m_settings->setValue( "rect/height", ui->height->value() );
    m_settings->setValue( "rect/frequency", ui->frequency->value() );
    m_settings->setValue( "rect/ontop", ui->onTop->isChecked() );
}

void AppWidget::slotOnTopChanged()
{
    Qt::WindowFlags flags = windowFlags();
    if ( ui->onTop->isChecked() )
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags &= ~Qt::WindowStaysOnTopHint;
    setWindowFlags( flags );
    show();
}

void AppWidget::slotCapParamsChanged()
{
    QRect captureRect( ui->left->value(), ui->top->value(), ui->width->value(), ui->height->value() );
    m_capture->setGeometry( captureRect );
    m_capture->setFrequency( ui->frequency->value() );
}

void AppWidget::slotProcessPixmap( const QPixmap & pixmap, const QPoint & cursor )
{
    // TODO
    //qWarning("GOTCHA!");
}

void AppWidget::on_btnGame_toggled( bool checked )
{
qWarning("%s %d", __PRETTY_FUNCTION__, checked);
}

void AppWidget::on_btnChallenge_toggled( bool checked )
{
qWarning("%s %d", __PRETTY_FUNCTION__, checked);
}

void AppWidget::on_btnLearn1_toggled( bool checked )
{
qWarning("%s %d", __PRETTY_FUNCTION__, checked);
}

void AppWidget::on_btnLearn2_toggled( bool checked )
{
qWarning("%s %d", __PRETTY_FUNCTION__, checked);
}

void AppWidget::on_letters_textChanged( const QString & text )
{
    // TEMP FOR TESTING
    QStringList words = m_scrambler->allWords( text, 3, 6 );
    ui->words->appendPlainText( words.join( "," ) );
}

