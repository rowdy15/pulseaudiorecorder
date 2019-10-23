#include "mainwindow.h"
#include <QLabel>
#include <QApplication>
#include <QProcess>



int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Pulse Audio Monitor Recorder");

    w.show();
    return a.exec();
}
