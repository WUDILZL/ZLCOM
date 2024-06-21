#include "ZLCom.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZLCom w;
    w.show();
    return a.exec();
}
