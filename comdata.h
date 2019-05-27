#ifndef COMDATA_H
#define COMDATA_H

#include <QDateTime>
#include "hidapi.h"
#include "libusb.h"
#include "usb.h"

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

union d_And_c
{
    double d;
    char c[8];
};

class ComData
{
public:
    explicit ComData();
    ~ComData();
    //
    // A utility to shift a new data value into a data array
    //
    static void shiftData_C(char *data, unsigned long long len, char newValue)
    {
        memmove(data, data + 1, sizeof(*data) * (len - 1));
        data[len - 1] = newValue;
    }
    static void shiftData_D(double *data, unsigned long long len, double newValue)
    {
        memmove(data, data + 1, sizeof(*data) * (len - 1));
        data[len - 1] = newValue;
    }

public:
    hid_device *myHandle;   // USB_HID

    QDateTime lastTime;     // 上次接收到数据的时间;用于判断是否更新表格为0
    // 串口收发数据头码
    unsigned long long headerLength;
    unsigned char *headerC; //   -89, 89, 62, -67 = (0xA7, 0x59, 0x3E, 0xBD)
    // The number of samples per data series used in this demo
    unsigned long long DataSize;
    double *d_timeStamps;	// The timestamps for the data series
    double *d_dataSeriesV;	// The values for the data series A
    double *d_dataSeriesA;	// The values for the data series B

    unsigned long long d_currentIndex;

private:



};


#endif // COMDATA_H
