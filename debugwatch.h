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

class DebugWatch : public QMainWindow
{
    Q_OBJECT
public:
    explicit DebugWatch(QWidget *parent = nullptr, ComData *comD = nullptr, USB_HID *hid = nullptr);
    ~DebugWatch();
signals:

public slots:
    void receive_Level_Num(int);
private slots:
    void onLedOnOffChanged(int);
    void onBtnOFC();
    void onBtnFSC();

private:
    void closeEvent ( QCloseEvent*);
    ComData *m_ComData;
    USB_HID *m_UsbHid;

    QLabel *m_TestInfo;         // 测试信息
};

#endif // DEBUGWATCH_H
