#ifndef DEBUGWATCH_H
#define DEBUGWATCH_H

#include <QMainWindow>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include "comdata.h"
#include "usb_hid.h"
#include <QButtonGroup>
#include <QCheckBox>

class DebugWatch : public QMainWindow
{
    Q_OBJECT
public:
    explicit DebugWatch(QWidget *parent = nullptr, ComData *comD = nullptr, USB_HID *hid = nullptr);
    ~DebugWatch();
signals:

public slots:
    void receive_Level_Num(int);
    void receive_Verify_Value();
private slots:
    void onLedOnOffChanged(int);
    void onBtnWriteFlash();
    void onBtnOFC();
    void onBtnFSC();
    void buttonGroupSlot(int);
    void onClickBox(bool);

private:
    void closeEvent ( QCloseEvent*);
    ComData *m_ComData;
    USB_HID *m_UsbHid;

    QLabel *m_TestInfo;         // 测试信息

    QLabel *m_Level1Verify_Min;
    QLabel *m_Level1Verify_Max;
    QLabel *m_Level2Verify_Min;
    QLabel *m_Level2Verify_Max;
    QLabel *m_Level3Verify_Min;
    QLabel *m_Level3Verify_Max;
    QLabel *m_Level4Verify_Min;
    QLabel *m_Level4Verify_Max;
};

#endif // DEBUGWATCH_H
