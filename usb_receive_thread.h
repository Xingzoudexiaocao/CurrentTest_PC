#ifndef USB_RECEIVE_THREAD_H
#define USB_RECEIVE_THREAD_H

#include <QObject>
#include <QThread>
#include <QtDebug>
#include "comdata.h"
#include "usb_hid.h"
#include "qchartviewer.h"

class USB_Receive_Thread : public QObject, public QThread
{
    Q_OBJECT
public:
    explicit USB_Receive_Thread(QObject *parent = nullptr, USB_HID *hid = nullptr, ComData *comD = nullptr);

signals:
    void get_USB_Data(unsigned char *buf, unsigned char len);
    void end_Thread();

protected:
    void run();

public:
    bool isStop;
private:
    ComData *m_ComData;
    USB_HID *m_UsbHid;

private:

};

#endif // USB_RECEIVE_THREAD_H
