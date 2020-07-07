#include "Aconity.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Aconity w;
	w.show();
	return a.exec();
}
