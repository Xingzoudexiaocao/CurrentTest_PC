#include "comdata.h"
#include <QDebug>
#include "mainwindow.h"

using namespace std;

ComData::ComData()
{
    myHandle = nullptr;
    DataSize = 360000;  // 保存1小时数据
    // 头码 167, 89, 62, 189 = (0xA7, 0x59, 0x3E, 0xBD)
    headerC = new unsigned char[4]{0xA7, 0x59, 0x3E, 0xBD};
    headerLength = 4;
//    memcpy(headerC, &tmp, headerLength);      // 初始化头码
    d_timeStamps = new double [DataSize];
    d_dataSeriesV = new double [DataSize];
    d_dataSeriesA = new double [DataSize];
    qDebug() << "ComData构造，分配内存空间。";

    ClearData();

    updataFile = new unsigned char[256000];     // 最大256k
//    memset(&updataFile, 0x00, sizeof(updataFile));
    updataFileLen = 0;

    if(!ReadData(IS_VERIFIED))
    {
        qDebug() << "读取是否校验值失败";
        SettingIsVerified = true;     // 初始化 已校验模式
        WriteData(IS_VERIFIED);
    }
    qDebug() << "SettingIsVerified = " << SettingIsVerified;
    if(!ReadData(AVERAGE_VALUE))
    {
        qDebug() << "读取平均值失败";
        SettingAverageTime = 1;     // 初始化 1分钟
        WriteData(AVERAGE_VALUE);
    }
    qDebug() << "SettingAverageTime = " << SettingAverageTime;
    if(!ReadData(BATTERY_CAPACITY_VALUE))
    {
        qDebug() << "读取电池容量失败";
        SettingBatteryCapacity = 5000.0;      // 初始化5000mAh
        WriteData(BATTERY_CAPACITY_VALUE);
    }
    qDebug() << "SettingAverageTime = " << SettingBatteryCapacity;
}

ComData::~ComData()
{
    delete [] d_timeStamps;
    delete [] d_dataSeriesV;
    delete [] d_dataSeriesA;
    qDebug() << "ComData析构，回收内存空间。" ;
}

void ComData::ClearData(void)
{
    for (unsigned long long i = 0; i < DataSize; i++) {
        *(d_timeStamps + i) = 0;
        *(d_dataSeriesV + i) = 0;
        *(d_dataSeriesA + i) = 0;
    }
    // 初始化平均值
    d_Avg_V = 0;
    d_Avg_A = 0;
    d_currentIndex = 1;     // 默认为1，这样会显示波形图

    BeginTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    lastTime = QDateTime::fromMSecsSinceEpoch(BeginTime);      // 更新接收时间
    memset(&AverageVolMinute, 0x00, sizeof(AverageVolMinute));
    memset(&AverageCurMinute, 0x00, sizeof(AverageCurMinute));
    AverageMinuteCount = 0;
    RunningCount = 0;

}

bool ComData::WriteData(int dataI)
{
    QSettings app("iSCAN", "UserSetting");
    //进入到"property"组中.如果没有下面这行代码,得到的值都为0.因为不存在嘛.
    app.beginGroup("property");

    switch(dataI)
    {
        case IS_VERIFIED:
            app.setValue("is_verified_value", SettingIsVerified);
            return 1;
        break;
        case AVERAGE_VALUE:
            app.setValue("average_value", SettingAverageTime);
            return 1;
        break;
        case BATTERY_CAPACITY_VALUE:
            app.setValue("battery_capacity_value", SettingBatteryCapacity);
            return 1;
            break;
        case ALL_DATA:
            break;
        default: break;
    }
    return 0;
}

bool ComData::ReadData(int dataI)
{
    QSettings app("iSCAN", "UserSetting");
    //进入到"property"组中.如果没有下面这行代码,得到的值都为0.因为不存在嘛.
    app.beginGroup("property");

    switch(dataI)
    {
        case IS_VERIFIED:
            if(app.value("is_verified_value").toString().length())
            {
                SettingIsVerified = app.value("is_verified_value").toBool();
                return 1;
            }
            else
                return 0;
        break;
        case AVERAGE_VALUE:
            if(app.value("average_value").toString().length())
            {
                SettingAverageTime = app.value("average_value").toLongLong();
                return 1;
            }
            else
                return 0;
        break;
        case BATTERY_CAPACITY_VALUE:
            if(app.value("battery_capacity_value").toString().length())
            {
                SettingBatteryCapacity = app.value("battery_capacity_value").toDouble();
                return 1;
            }
            else
                return 0;
            break;
        case ALL_DATA:
            break;
        default: break;
    }
    return 0;
}
