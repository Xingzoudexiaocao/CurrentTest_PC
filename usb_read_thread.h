#ifndef USB_READ_THREAD_H
#define USB_READ_THREAD_H
#include <QObject>
#include <QThread>
#include <QtDebug>
#include "comdata.h"
#include "usb_hid.h"
#include "qchartviewer.h"
#include "mainwindow.h"

class USB_Read_Thread: public QThread
{

public:
    USB_Read_Thread(USB_HID *hid, ComData *comD);

signals:
    void get_USB_Data(unsigned char *buf, unsigned char len);

protected:
    void run();

private:
    ComData *m_ComData;
    USB_HID *m_UsbHid;

private:
    void shiftData_D(double *data, unsigned long long len, double newValue);
//    void get_USB_Data(unsigned char *buf, unsigned char len);
};

#endif // USB_READ_THREAD_H
