#include "debugwatch.h"
#include <QDebug>

using namespace std;

DebugWatch::DebugWatch(QWidget *parent, ComData *comD, USB_HID *hid) : QMainWindow(parent)
{
    qDebug() << "DebugWatch构造函数。";
    m_ComData = comD;
    m_UsbHid = hid;

    // 测试档位
    QComboBox *updatePeriod = new QComboBox(this);
    updatePeriod->setGeometry(6, 20, 150, 40);
    updatePeriod->addItems(QStringList() << "请选择" << "LEVEL_1" << "LEVEL_2" << "LEVEL_3" << "LEVEL_4");
    connect(updatePeriod, SIGNAL(currentIndexChanged(int)),
            SLOT(onLedOnOffChanged(int)));
    // 显示档位
    m_TestInfo = new QLabel(this);
    m_TestInfo->setGeometry(6, 40, 300, 60);
    m_TestInfo->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:14px; color:red;}");
    m_TestInfo->setText("当前档位：");
    // Run push button
    QPushButton *btnOFC = new QPushButton("校准偏移", this);
    btnOFC->setGeometry(6, 200, 150, 40);
    btnOFC->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    connect(btnOFC, SIGNAL(clicked(bool)), SLOT(onBtnOFC()));
    // Run push button
    QPushButton *btnFSC = new QPushButton("校准增益", this);
    btnFSC->setGeometry(6, 240, 150, 40);
    btnFSC->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    connect(btnFSC, SIGNAL(clicked(bool)), SLOT(onBtnFSC()));
    // 注意事项
    QLabel *tips = new QLabel(this);
    tips->setGeometry(2, 280, 350, 60);
    tips->setStyleSheet("QLabel { text-align:left; padding:10px; font-size:14px; color:red;}");
    tips->setText("*偏移校准时，请保持AD输入电压为0\n*增益校准时，请保持AD输入电压为最大值Vref");


}

DebugWatch::~DebugWatch()
{
    qDebug() << "DebugWatch析构函数。";

}

void DebugWatch::onLedOnOffChanged(int index)
{
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        QMessageBox::about(this, "提示", "USB设备未打开！");
        return;
    }
    unsigned char sendP[32];
    memset(sendP, 0, sizeof (sendP));
    sendP[0] = 0xa5; sendP[1] = 0xb7; sendP[2] = 0xa5; sendP[3] = 0xb7;

    switch (index) {
        case 1: sendP[4] = 0x01; qDebug() << "Set Level 1！"; break;
        case 2: sendP[4] = 0x02; qDebug() << "Set Level 2！"; break;
        case 3: sendP[4] = 0x03; qDebug() << "Set Level 3！"; break;
        case 4: sendP[4] = 0x04; qDebug() << "Set Level 4！"; break;
        default: break;
    }
    m_UsbHid->SendUSB(sendP, 32);   // 使用USB发送数据
}
void DebugWatch::onBtnOFC()
{
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        QMessageBox::about(this, "提示", "USB设备未打开！");
        return;
    }
    unsigned char sendP[32];
    memset(sendP, 0, sizeof (sendP));
    sendP[0] = 0xa5; sendP[1] = 0xb7; sendP[2] = 0xa5; sendP[3] = 0xb7;
    sendP[4] = 0x05; qDebug() << "Calibrate OFC";
    m_UsbHid->SendUSB(sendP, 32);   // 使用USB发送数据
}
void DebugWatch::onBtnFSC()
{
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        QMessageBox::about(this, "提示", "USB设备未打开！");
        return;
    }
    unsigned char sendP[32];
    memset(sendP, 0, sizeof (sendP));
    sendP[0] = 0xa5; sendP[1] = 0xb7; sendP[2] = 0xa5; sendP[3] = 0xb7;
    sendP[4] = 0x06; qDebug() << "Calibrate FSC";
    m_UsbHid->SendUSB(sendP, 32);   // 使用USB发送数据
}

void DebugWatch::receive_Level_Num(int val)
{
    switch (val) {
        case 1: m_TestInfo->setText("当前档位：1 100k欧姆"); break;
        case 2: m_TestInfo->setText("当前档位：2 1k欧姆"); break;
        case 3: m_TestInfo->setText("当前档位：3 10欧姆"); break;
        case 4: m_TestInfo->setText("当前档位：4 0.1欧姆"); break;
        default: m_TestInfo->setText("当前档位：断开");  return;   // 档位不正确，直接返回
    }

}
