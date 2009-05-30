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

#include "AppWidget.h"
#include "ui_AppWidget.h"
#include "Capture.h"
#include "GameState.h"
#include "InputUtils.h"
#include "ocr/Ocr.h"
#include <QApplication>
#include <QButtonGroup>
#include <QDesktopWidget>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QSettings>
#include <QDirIterator>
#include <QTimer>
#include <QDebug>

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
    , m_capture( 0 )
    , m_game( 0 )
    , m_ocr( 0 )
{
    // create ui
    ui->setupUi(this);
    QDesktopWidget dw;
    ui->display1->setFixedSize( 64, 64 );
    ui->display2->setFixedSize( 320, 41 );
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

    // create the capture
    m_capture = new Capture( this );
    connect( m_capture, SIGNAL(gotPixmap(const QPixmap &, const QPoint &)),
             this, SLOT(slotProcessPixmap(const QPixmap &, const QPoint &)) );
    slotCapParamsChanged();

    // create the OCR, train with font and saved glyphs
    m_ocr = new Ocr();

    QFont font( "Arial", 32 );
    font.setBold( true );
    m_ocr->trainFont( font );

    QDirIterator dIt( QCoreApplication::applicationDirPath() + QDir::separator() + "wc-glyphs", QStringList() << "glyph_*.png", QDir::Files );
    while ( dIt.hasNext() ) {
        QString fileName = dIt.next();
        QChar character = fileName.right( 5 ).at( 0 );
        QImage image( fileName, "PNG" );
        if ( !image.isNull() ) {
            m_ocr->trainGlyph( image, character );
            qWarning() << "loaded" << fileName << character;
        }
    }

    // ### TEMP
    m_capture->setEnabled( true );
}

AppWidget::~AppWidget()
{
    saveSettings();
    delete m_settings;
    delete m_ocr;
    delete m_game;
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

void AppWidget::slotProcessPixmap( const QPixmap & pixmap, const QPoint & /*cursor*/ )
{
    QPixmap pix = pixmap;
    QPainter pp( &pix );
    pp.setPen( Qt::red );
    pp.drawRect( 121, 248, 35 - 1, 37 - 1 );
    pp.drawRect( 175, 248, 35 - 1, 37 - 1 );
    pp.drawRect( 230, 248, 35 - 1, 37 - 1 );
    pp.drawRect( 284, 248, 35 - 1, 37 - 1 );
    pp.drawRect( 338, 248, 35 - 1, 37 - 1 );
    pp.drawRect( 393, 248, 35 - 1, 37 - 1 );
    pp.end();
    ui->display1->setPixmap( pix.copy( 0, 0, 16, 16 ).scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::FastTransformation ) );
    ui->display2->setPixmap( pix.copy( 114, 245, 320, 41 ) );
}

void AppWidget::on_btnGame_toggled( bool checked )
{
    qWarning("%s %d", __PRETTY_FUNCTION__, checked);

    delete m_game;
    m_game = 0;
    if ( checked )
        m_game = new GameState( ui, m_capture, m_ocr, this );
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

void AppWidget::on_trainButton_clicked()
{
    // find out the string
    QString letters = ui->trainLetters->text();
    QImage gamePixmap = m_capture->currentPixmap().toImage();
    int pixLetters[ 6 ] = { 121, 175, 230, 284, 338, 393 };
    int pixLetterTop = 248;
    int pixLetterWidth = 35;
    int pixLetterHeight = 37;
    for ( int i = 0; i < 6; i++ ) {
        QImage letter = gamePixmap.copy( pixLetters[ i ], pixLetterTop, pixLetterWidth, pixLetterHeight );
        QChar character = letters.at( i );
        letter.save( QString( "wc-glyphs/glyph_%1.png" ).arg( character.toLatin1() ), "PNG" );
        m_ocr->trainGlyph( letter, character );
    }
}
