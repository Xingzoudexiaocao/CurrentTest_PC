#ifndef CALCULATE_TREAD_H
#define CALCULATE_TREAD_H

#include <QObject>
#include <QThread>
#include <QtDebug>
#include <QDebug>
#include "comdata.h"
#include "usb_hid.h"

class Calculate_Tread : public QThread
{
    Q_OBJECT
public:
    explicit Calculate_Tread(QObject *parent = nullptr, USB_HID *hid = nullptr, ComData *comD = nullptr);
    ~Calculate_Tread();

signals:
    void end_Thread();
    void signalUpdateCurAverage(qint64, double, double, double);
    void signalUpdateVolAverage(qint64, double, double, double);
    void signalUpdateWorkingTime(qint64, qint64, qint64);

protected:
    void run();

public:
    bool isStop;

private:
    ComData *m_ComData;
    USB_HID *m_UsbHid;


};

#endif // CALCULATE_TREAD_H
