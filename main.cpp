#include "mainwindow.h"
#include "realtime.h"
#include <QApplication>
#include <QDebug>
#include "comdata.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyleSheet("* {font-family:arial;font-size:11px}");
    ComData *comD = new ComData();

//    RealTime *demo = new RealTime(new QDialog(), comD);
//    demo->setStyleSheet("* {font-family:arial;font-size:15px}");
//    demo->setGeometry(400, 20, 960, 850);
//    demo->show();
//    RealTime demo(new QWidget(), comD);
//    demo.setStyleSheet("* {font-family:arial;font-size:15px}");
//    demo.setGeometry(400, 20, 960, 850);
//    demo.show();

    MainWindow w(new QWidget(), comD);
    w.show();

    qDebug() << "int型数据大小：" << sizeof (int) ;
    qDebug() << "double型数据大小：" << sizeof (double) ;



    return a.exec();
}
