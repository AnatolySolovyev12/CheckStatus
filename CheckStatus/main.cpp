#include "CheckStatus.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CheckStatus w;
    w.show();
    return a.exec();
}
