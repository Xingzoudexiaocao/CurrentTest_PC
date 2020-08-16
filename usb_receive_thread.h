#ifndef USB_RECEIVE_THREAD_H
#define USB_RECEIVE_THREAD_H

#include <QObject>
#include <QThread>
#include <QtDebug>
#include <QDebug>
#include "comdata.h"
#include "usb_hid.h"
#include "qchartviewer.h"

class USB_Receive_Thread : public QThread
{
    Q_OBJECT
public:
    explicit USB_Receive_Thread(QObject *parent = nullptr, USB_HID *hid = nullptr, ComData *comD = nullptr);
    ~USB_Receive_Thread();

signals:
    void send_Level_Num(int);   // 发送档位信息
    void get_Vol_Cur_Now(qint64, double, double);
    void get_USB_Data(QDateTime, double, unsigned char, unsigned char);
    void end_Thread();
    void setAckOrNak(int);
    void get_Version_Length(unsigned long long, unsigned long long);
    void get_Verify_Value(void);
    void get_RandomKey_Value(void);

protected:
    void run();

public:
    bool isStop;
private:
    ComData *m_ComData;
    USB_HID *m_UsbHid;

private:
    void HandleData(ST_REC_STRUCT *);
    void CreateRandomKey(void);

};

#endif // USB_RECEIVE_THREAD_H
