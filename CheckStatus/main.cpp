#include "CheckStatus.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CheckStatus w;

	w.setWindowIcon(QIcon("icon.png"));
	w.setWindowTitle("CheckStatus by Solovev");
    w.show();
    return a.exec();
}