#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <QtSerialPort/QSerialPort>        //提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>    //提供系统中存在的串口的信息
#include <QTextCodec>

#include "realtime.h"
#include "comdata.h"
#include "debugwatch.h"

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, ComData *comD = nullptr, USB_HID *hid = nullptr);
    ~MainWindow();
public Q_SLOTS:
    void colseDebug();
private Q_SLOTS:
//    void on_OpenCom_clicked();
//    void on_SearchCom_clicked();
//    void on_SendData_clicked();
//    void on_ClearData_clicked();
//    void on_AutoSend_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
    ComData *m_ComData;
    USB_HID *m_UsbHid;
    RealTime *demo;
    DebugWatch *m_Debug;        // 调试监控窗口
    char RecBuffer[1024];
    unsigned long long RecIndex;


protected:
    virtual void keyPressEvent(QKeyEvent *ev);      // 键盘事件
    virtual void keyReleaseEvent(QKeyEvent *ev);
    virtual void resizeEvent(QResizeEvent *event);
    void OpenDebugWindow();

};

#endif // MAINWINDOW_H
