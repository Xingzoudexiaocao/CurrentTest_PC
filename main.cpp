#include "mainwindow.h"
#include "realtime.h"
#include <QApplication>
#include <QDebug>
#include "comdata.h"

using namespace std;
// windeployqt --qmldir  C:\Users\ThinkPad\Documents\Test Test.exe
// windeployqt iSCAN.exe

void printscreeninfo(void)

{
    QDesktopWidget *dwsktopwidget = QApplication::desktop();
    QRect deskrect = dwsktopwidget->availableGeometry();
    QRect screenrect = dwsktopwidget->screenGeometry();
//    QDesktopWidget *dwsktopwidget = QApplication::desktop();
//    QRect deskrect = dwsktopwidget->availableGeometry();
//    QRect screenrect = dwsktopwidget->screenGeometry();
    int scrcount = dwsktopwidget->screenCount();
    qCritical("screenrect.w==%s\n",qPrintable(QString::number(screenrect.width())));
    qCritical("screenrect.h==%s\n",qPrintable(QString::number(screenrect.height())));
    qCritical("deskrect.w==%s\n",qPrintable(QString::number(deskrect.width())));
    qCritical("deskrect.h==%s\n",qPrintable(QString::number(deskrect.height())));
    qCritical("scrcount==%s\n",qPrintable(QString::number(scrcount)));

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyleSheet("* {font-family:arial;font-size:11px}");
    ComData *comD = new ComData();
    USB_HID *hid = new USB_HID(comD);
//    RealTime *demo = new RealTime(new QDialog(), comD);
//    demo->setStyleSheet("* {font-family:arial;font-size:15px}");
//    demo->setGeometry(400, 20, 960, 850);
//    demo->show();
//    RealTime demo(new QWidget(), comD);
//    demo.setStyleSheet("* {font-family:arial;font-size:15px}");
//    demo.setGeometry(400, 20, 960, 850);
//    demo.show();

    printscreeninfo();

    MainWindow w(nullptr, comD, hid);
//    w.setWindowFlags( Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // 置顶，最小化，关闭   | Qt::WindowMaximizeButtonHint     Qt::WindowStaysOnTopHint |
//    w.setWindowFlags(Qt::FramelessWindowHint);    // 无边框
//    w.setWindowOpacity(0.5);
    w.show();

//    RealTime *demo = new RealTime(nullptr, comD, hid);
//    demo->show();

//    DebugWatch *demo = new DebugWatch(new QDialog(), comD);
//    demo->setStyleSheet("* {font-family:arial;font-size:15px}");
//    demo->setGeometry(400, 200, 100, 200);
//    demo->show();
//        DebugWatch demo(new QWidget(), comD);
//        demo.setStyleSheet("* {font-family:arial;font-size:15px}");
//        demo.setGeometry(400, 20, 360, 350);
//        demo.show();

//    qDebug() << "int型数据大小：" << sizeof (int) ;
//    qDebug() << "double型数据大小：" << sizeof (double) ;



    return a.exec();
}
