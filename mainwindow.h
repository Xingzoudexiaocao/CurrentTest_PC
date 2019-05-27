#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <QtSerialPort/QSerialPort>        //提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>    //提供系统中存在的串口的信息
#include <QTextCodec>

#include "comdata.h"

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
    explicit MainWindow(QWidget *parent = nullptr, ComData *comD = nullptr);
    ~MainWindow();

private Q_SLOTS:
    void serialPort_readyRead();
    void on_OpenCom_clicked();
    void on_SearchCom_clicked();
    void on_SendData_clicked();
    void on_ClearData_clicked();
    void on_AutoSend_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
    ComData *m_ComData;
    char RecBuffer[1024];
    unsigned long long RecIndex;
};

#endif // MAINWINDOW_H
