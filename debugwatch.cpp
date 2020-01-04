#include "debugwatch.h"
#include <QDebug>

using namespace std;

DebugWatch::DebugWatch(QWidget *parent, ComData *comD, USB_HID *hid) : QMainWindow(parent)
{
    qDebug() << "DebugWatch构造函数。";
    m_ComData = comD;
    m_UsbHid = hid;

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint); // 置顶，最小化，关闭

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
    QPushButton *btnWriteFlash = new QPushButton("测试写Flash", this);
    btnWriteFlash->setGeometry(6, 120, 150, 40);
    btnWriteFlash->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    connect(btnWriteFlash, SIGNAL(clicked(bool)), SLOT(onBtnWriteFlash()));
    // Run push button
    QPushButton *btnOFC = new QPushButton("校准偏移", this);
    btnOFC->setGeometry(6, 200, 150, 40);
    btnOFC->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    connect(btnOFC, SIGNAL(clicked(bool)), SLOT(onBtnOFC()));
    // Run push button
    QPushButton *btnFSC = new QPushButton("校准增益", this);
    btnFSC->setGeometry(160, 200, 150, 40);
    btnFSC->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    connect(btnFSC, SIGNAL(clicked(bool)), SLOT(onBtnFSC()));
    // 注意事项
    QLabel *tips = new QLabel(this);
    tips->setGeometry(2, 240, 350, 60);
    tips->setStyleSheet("QLabel { text-align:left; padding:10px; font-size:14px; color:red;}");
    tips->setText("*偏移校准时，请保持AD输入电压为0\n*增益校准时，请保持AD输入电压为最大值Vref");

//
    QPushButton *btnLevel1Min = new QPushButton("校准档位1最小值", this);
    btnLevel1Min->setGeometry(6, 300, 190, 40);
    btnLevel1Min->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    QPushButton *btnLevel1Max = new QPushButton("校准档位1最大值", this);
    btnLevel1Max->setGeometry(200, 300, 190, 40);
    btnLevel1Max->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    QPushButton *btnLevel2Min = new QPushButton("校准档位2最小值", this);
    btnLevel2Min->setGeometry(6, 340, 190, 40);
    btnLevel2Min->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    QPushButton *btnLevel2Max = new QPushButton("校准档位2最大值", this);
    btnLevel2Max->setGeometry(200, 340, 190, 40);
    btnLevel2Max->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    QPushButton *btnLevel3Min = new QPushButton("校准档位3最小值", this);
    btnLevel3Min->setGeometry(6, 380, 190, 40);
    btnLevel3Min->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    QPushButton *btnLevel3Max = new QPushButton("校准档位3最大值", this);
    btnLevel3Max->setGeometry(200, 380, 190, 40);
    btnLevel3Max->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    QPushButton *btnLevel4Min = new QPushButton("校准档位4最小值", this);
    btnLevel4Min->setGeometry(6, 420, 190, 40);
    btnLevel4Min->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    QPushButton *btnLevel4Max = new QPushButton("校准档位4最大值", this);
    btnLevel4Max->setGeometry(200, 420, 190, 40);
    btnLevel4Max->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    QButtonGroup *m_ButtonGroup = new QButtonGroup(this);
    m_ButtonGroup->addButton(btnLevel1Min, 1); m_ButtonGroup->addButton(btnLevel1Max, 2);
    m_ButtonGroup->addButton(btnLevel2Min, 3); m_ButtonGroup->addButton(btnLevel2Max, 4);
    m_ButtonGroup->addButton(btnLevel3Min, 5); m_ButtonGroup->addButton(btnLevel3Max, 6);
    m_ButtonGroup->addButton(btnLevel4Min, 7); m_ButtonGroup->addButton(btnLevel4Max, 8);
    connect(m_ButtonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonGroupSlot(int)));
}

DebugWatch::~DebugWatch()
{
    qDebug() << "DebugWatch析构函数。";

}

void DebugWatch::closeEvent ( QCloseEvent * e )
{
    this->setAttribute(Qt::WA_DeleteOnClose);
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
void DebugWatch::onBtnWriteFlash()
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
    sendP[4] = 0x08; qDebug() << "Wirte Flash Test.";
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

void DebugWatch::buttonGroupSlot(int level)
{
//    qDebug() << "测试按键组！" << level;
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        QMessageBox::about(this, "提示", "USB设备未打开！");
        return;
    }
    unsigned char sendP[32];
    memset(sendP, 0, sizeof (sendP));
    sendP[0] = 0xa5; sendP[1] = 0xb7; sendP[2] = 0xa5; sendP[3] = 0xb7;
    sendP[4] = level + 0x10;
    qDebug() << "Calibrate Button Group" << level;
    m_UsbHid->SendUSB(sendP, 32);   // 使用USB发送数据
}
