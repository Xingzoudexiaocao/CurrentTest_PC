#ifndef COMDATA_H
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

#include "iconhelper.h"
#include "myhelper.h"

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#define iSCAN_STM32     1       // 产品for STM32
#define iSCAN_ARTERY    2       // 产品for ARTERY
#define iSCAN_INTERNAL    3       // 产品for INTERNAL 内部使用
#define MCU_TYPE  iSCAN_ARTERY
//#define appUpdataDebug    // APP升级部分区别，调试时用,判断是否对bin文件加密处理

#if (MCU_TYPE == iSCAN_INTERNAL)
    #define isDebuf           // 是否调试模式
#endif

#define DIFFER_COUNT_VALUE      1000

#define YMODEM_SOH (0x01)
#define YMODEM_STX (0x02)
#define YMODEM_EOT (0x04)
#define YMODEM_ACK (0x06)
#define YMODEM_NAK (0x15)
#define YMODEM_VER_LEN (0xA3)
#define YMODEM_TIMEOUT (0x7B)
#define YMODEM_VALID_VALUE_1 (0x56)
#define YMODEM_VALID_VALUE_2 (0x59)
//#define YMODEM_CAN (0x18)
//#define YMODEM_C (0x43)

static QString compangyName = "一起努力";
static QString productName = "iSCAN";            // 精密电流测量仪
static QString appVersionName = "100.000.208";

typedef struct HISTORY_DATAINFO_STRUCT
{
    double average;
    double maximun;
    double minimun;
} HISTORY_DATAINFO_STRUCT;

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

typedef struct DB_VERIFY_STRUCT
{
    quint32 Lelve_1_min;
    quint32 Lelve_1_max;
    quint32 Lelve_2_min;
    quint32 Lelve_2_max;
    quint32 Lelve_3_min;
    quint32 Lelve_3_max;
    quint32 Lelve_4_min;
    quint32 Lelve_4_max;
} DB_VERIFY_STRUCT;

typedef struct DB_STANDARD_STRUCT
{
    double Lelve_1_min;
    double Lelve_1_max;
    double Lelve_2_min;
    double Lelve_2_max;
    double Lelve_3_min;
    double Lelve_3_max;
    double Lelve_4_min;
    double Lelve_4_max;
} DB_STANDARD_STRUCT;

typedef struct DB_CALCULATE_STRUCT
{
    double voltageSum = 0;
    double currentSum = 0;
    qint64 countSum = 0;

    double energySum = 0;
    double currentSumSecond = 0;

    qint16 differCountBegin = 0;        // 16位
    qint16 differCountEnd = 0;
    double differEnergy = 0;
} DB_CALCULATE_STRUCT;

union d_And_c
{
    double d;
    char c[8];
};

typedef enum{
    IS_VERIFIED,
    AVERAGE_VALUE,
    BATTERY_CAPACITY_VALUE,
    DIFFER_V_BEGIN,
    DIFFER_V_END,
    ALL_DATA
} SETTINGDATA;

class ComData
{
public:
    explicit ComData();
    ~ComData();
    //
    // A utility to shift a new data value into a data array
    //
    static void shiftData_C(char *data, qint64 len, char newValue)
    {
        memmove(data, data + 1, sizeof(*data) * (len - 1));
        data[len - 1] = newValue;
    }
    static void shiftData_D(double *data, qint64 len, double newValue)
    {
        memmove(data, data + 1, sizeof(*data) * (len - 1));
        data[len - 1] = newValue;
    }
    void ClearData(void);
    bool WriteData(int);
    bool ReadData(int);

public:
    hid_device *myHandle;   // USB_HID

//    QRect *gUiRect;
    QSize *gUiSize;

    bool layerIsPause;
    qint64 layer_BeginTime;
    qint64 layer_currentIndex;
    double *layer_timeStamps;	// The timestamps for the data series
    double *layer_dataSeriesV;	// The values for the data series A
    double *layer_dataSeriesA;	// The values for the data series B

    qint64 T1_Cur_Index;        // 电流波形T1起始索引
    qint64 T2_Cur_Index;        // 电流波形T2起始索引
    qint64 T1_Vol_Index;        // 电压波形T1起始索引
    qint64 T2_Vol_Index;        // 电压波形T2起始索引
    QDateTime lastTime;     // 上次接收到数据的时间;用于判断是否更新表格为0
    qint64 BeginTime;
    qint64 RunningCount;    // 运行时间计数
    double AverageVolMinute[60];        // 每个一分钟平均电压值，最多60分钟
    double AverageCurMinute[60];        // 每个一分钟平均电流值，最多60分钟
    qint64 AverageMinuteCount;          // 每个一分钟计数
    bool SettingIsVerified;             // 是否已校验
    qint64 SettingAverageTime;          // 设定的平均值计算时间
    double SettingBatteryCapacity;      // 设定的电池容量
    double UseringBatteryCapacity;      // 已使用电池容量
    double SettingDifferVBegin;         // ^V电量计算起始电压
    double SettingDifferVEnd;           // ^V电量计算截止电压
    // 串口收发数据头码
    unsigned long long headerLength;
    unsigned char *headerC; //   -89, 89, 62, -67 = (0xA7, 0x59, 0x3E, 0xBD)
    // The number of samples per data series used in this demo
    qint64 DataSize;
    double *d_timeStamps;	// The timestamps for the data series
    double *d_dataSeriesV;	// The values for the data series A
    double *d_dataSeriesA;	// The values for the data series B

    double d_Avg_V;
    double d_Avg_A;

    qint64 d_currentIndex;

    unsigned char *updataFile;
    unsigned long long updataFileLen;

    DB_VERIFY_STRUCT d_verifyValue;
    DB_STANDARD_STRUCT d_standardValue;
    DB_CALCULATE_STRUCT d_calculateValue;
private:



};


#endif // COMDATA_H
