#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QtGui/QWidget>

namespace Ui
{
    class AppWidgetClass;
}

class AppWidget : public QWidget
{
    Q_OBJECT

public:
    AppWidget(QWidget *parent = 0);
    ~AppWidget();

private:
    Ui::AppWidgetClass *ui;
};

#endif // APPWIDGET_H
