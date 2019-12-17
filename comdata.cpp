#include "comdata.h"
#include <QDebug>
#include "mainwindow.h"

using namespace std;

ComData::ComData()
{
    myHandle = nullptr;
    lastTime = QDateTime::currentDateTime();
    d_currentIndex = 1;     // 默认为1，这样会显示波形图
    DataSize = 360000;  // 保存1小时数据
    // 头码 167, 89, 62, 189 = (0xA7, 0x59, 0x3E, 0xBD)
    headerC = new unsigned char[4]{0xA7, 0x59, 0x3E, 0xBD};
    headerLength = 4;
//    memcpy(headerC, &tmp, headerLength);      // 初始化头码
    d_timeStamps = new double [DataSize];
    d_dataSeriesV = new double [DataSize];
    d_dataSeriesA = new double [DataSize];
    qDebug() << "ComData构造，分配内存空间。";
    for (unsigned long long i = 0; i < DataSize; i++) {
        *(d_timeStamps + i) = 0;
        *(d_dataSeriesV + i) = 0;
        *(d_dataSeriesA + i) = 0;
    }
    // 初始化平均值
    d_Avg_V = 0;
    d_Avg_A = 0;

    updataFile = new unsigned char[256000];     // 最大256k
//    memset(&updataFile, 0x00, sizeof(updataFile));
    updataFileLen = 0;
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
    d_currentIndex = 0;     // 默认为1，这样会显示波形图
    lastTime = QDateTime::currentDateTime();
}
