#include "AppWidget.h"
#include "ui_AppWidget.h"

AppWidget::AppWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AppWidgetClass)
{
    ui->setupUi(this);
}

AppWidget::~AppWidget()
{
    delete ui;
}
