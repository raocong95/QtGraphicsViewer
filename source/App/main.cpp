#include "MainWindow.h"
#include <QApplication>
#include <GraphicsScene.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("Howard");
	QCoreApplication::setOrganizationDomain("honghuali.github.io");
	QCoreApplication::setApplicationName("GraphicsTools");

    MainWindow w;
    w.setWindowIcon(QIcon(":/media/icon.png"));
    w.setWindowTitle("QtGraphicsViewer");

    return a.exec();
}
