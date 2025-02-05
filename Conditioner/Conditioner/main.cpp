#include "CustomMainWindow.h"
#include "AppData.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(appIcon));
	CustomMainWindow w;
	w.show();
	return a.exec();
}
