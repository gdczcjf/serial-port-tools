#include <QApplication>
#include "main_window/main_window.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("serialPortTools");

    MainWindow w;
    w.show();
    return a.exec();
}
