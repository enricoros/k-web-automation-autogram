#include <QtGui/QApplication>
#include "AppWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppWidget w;
    w.show();
    return a.exec();
}
