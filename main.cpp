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
//    QRect screenrect_2 = dwsktopwidget->screenGeometry(1);
    int scrcount = dwsktopwidget->screenCount();
    qCritical("screenrect.w==%s\n",qPrintable(QString::number(screenrect.width())));
    qCritical("screenrect.h==%s\n",qPrintable(QString::number(screenrect.height())));
//    qCritical("screenrect_2.w==%s\n",qPrintable(QString::number(screenrect_2.width())));
//    qCritical("screenrect_2.h==%s\n",qPrintable(QString::number(screenrect_2.height())));
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

    QDesktopWidget *dwsktopwidget = QApplication::desktop();
//    QRect deskrect = dwsktopwidget->availableGeometry();
    QRect screenrect = dwsktopwidget->screenGeometry();
//    printscreeninfo();
    if(screenrect.width() >= 1920 && screenrect.height() >= 1080) {
        comD->gUiSize->setWidth(1920);
        comD->gUiSize->setHeight(1040);
    } else if(screenrect.width() >= 1440 && screenrect.height() >= 900) {
        comD->gUiSize->setWidth(1440);
        comD->gUiSize->setHeight(860);
    } else {
        comD->gUiSize->setWidth(1280);
        comD->gUiSize->setHeight(768);
    }
    qDebug() << "UI界面大小：" << comD->gUiSize->width() << comD->gUiSize->height();

    myHelper::SetUTF8Code();
    #if (MCU_TYPE == iSCAN_STM32)
        myHelper::SetStyle("black_style");//黑色风格
    #elif  (MCU_TYPE == iSCAN_ARTERY)
        myHelper::SetStyle("white_style");//白色风格
    #elif  (MCU_TYPE == iSCAN_INTERNAL)
        myHelper::SetStyle("white_style");//白色风格
    #endif

//    QFile file(":/black_style.css");
//    if (file.open(QFile::ReadOnly)) {
//        qDebug() << "文件打开成功。";
////        QString qss = QLatin1String(file.readAll());
//        QString qss = file.readAll();
//        qDebug() << "qss = " << qss;
//        QString paletteColor = qss.mid(20, 7);
//        qDebug() << "paletteColor = " << paletteColor;
//        qApp->setPalette(QPalette(QColor(paletteColor)));
//        qApp->setStyleSheet(qss);
//        file.close();
//    }
    myHelper::SetChinese();

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
