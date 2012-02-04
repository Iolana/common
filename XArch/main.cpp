#include "xarch.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setApplicationName("XArch"); 
	XArch w;
	w.show();
	return a.exec();
}
