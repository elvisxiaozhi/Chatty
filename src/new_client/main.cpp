#include "widget.h"
#include <QApplication>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    QApplication::setDesktopSettingsAware(false);
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
