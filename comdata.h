﻿#ifndef COMDATA_H
#define COMDATA_H

#include <QDateTime>
#include "hidapi.h"
#include "libusb.h"
#include "usb.h"
#include <math.h>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQueryModel>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#define isDebuf           // 是否调试模式
#define appUpdataDebug    // APP升级部分区别，调试时用,判断是否对bin文件加密处理

#define YMODEM_SOH (0x01)
#define YMODEM_STX (0x02)
#define YMODEM_EOT (0x04)
#define YMODEM_ACK (0x06)
#define YMODEM_NAK (0x15)
#define YMODEM_VER_LEN (0xA3)
#define YMODEM_TIMEOUT (0x7B)
//#define YMODEM_CAN (0x18)
//#define YMODEM_C (0x43)

typedef struct ST_REC_STRUCT
{
    unsigned char rec[32];
} ST_REC_STRUCT;

typedef struct DB_WRITE_STRUCT
{
    qint64 time;
    double vol;
    double cur;
} DB_WRITE_STRUCT;

union d_And_c
{
    double d;
    char c[8];
};

typedef enum{
    AVERAGE_VALUE,
    BATTERY_CAPACITY_VALUE,
    ALL_DATA
} SETTINGDATA;

static QString compangyName = "一起努力";
static QString productName = "精密电流测量仪";

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
    void ClearData(void);
    bool WriteData(int);
    bool ReadData(int);

public:
    hid_device *myHandle;   // USB_HID

    QDateTime lastTime;     // 上次接收到数据的时间;用于判断是否更新表格为0
    qint64 BeginTime;
    qint64 RunningCount;    // 运行时间计数
    double AverageVolMinute[60];        // 每个一分钟平均电压值，最多60分钟
    double AverageCurMinute[60];        // 每个一分钟平均电流值，最多60分钟
    qint64 AverageMinuteCount;          // 每个一分钟计数
    qint64 SettingAverageTime;          // 设定的平均值计算时间
    qint64 SettingBatteryCapacity;      // 设定的电池容量
    // 串口收发数据头码
    unsigned long long headerLength;
    unsigned char *headerC; //   -89, 89, 62, -67 = (0xA7, 0x59, 0x3E, 0xBD)
    // The number of samples per data series used in this demo
    unsigned long long DataSize;
    double *d_timeStamps;	// The timestamps for the data series
    double *d_dataSeriesV;	// The values for the data series A
    double *d_dataSeriesA;	// The values for the data series B

    double d_Avg_V;
    double d_Avg_A;

    unsigned long long d_currentIndex;

    unsigned char *updataFile;
    unsigned long long updataFileLen;

private:



};


#endif // COMDATA_H
