#include "mainwindow.h"
#include <QApplication>
#include <QComboBox>
#include "test.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("QMainWindow {background: 'white';}");
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();



    return a.exec();
}
