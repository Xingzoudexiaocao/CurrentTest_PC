#include "calculate_tread.h"

Calculate_Tread::Calculate_Tread(QObject *parent, USB_HID *hid, ComData *comD) : QThread ()
{
    qDebug("计算线程create: %d", this->currentThreadId());
    m_UsbHid = hid;
    m_ComData = comD;
    isStop = false;
}

Calculate_Tread::~Calculate_Tread()
{
     qDebug() << "Calculate_Tread析构函数。";
}

void Calculate_Tread::run()
{
    qDebug("计算线程run: %d", this->currentThreadId());
    qint64 CurDifferT_A = 0;
    qint64 CurDifferT_V = 0;
    while (!isStop) {

        try {
            if(m_ComData->T1_Cur_Index > 0 && m_ComData->T1_Cur_Index <= m_ComData->layer_currentIndex &&
                    m_ComData->T2_Cur_Index > 0 && m_ComData->T2_Cur_Index <= m_ComData->layer_currentIndex &&
                    CurDifferT_A != m_ComData->T2_Cur_Index - m_ComData->T1_Cur_Index)
            {
                CurDifferT_A = m_ComData->T2_Cur_Index - m_ComData->T1_Cur_Index;

                double sunCurrent = 0;

                if(CurDifferT_A == 0)
                    emit signalUpdateCurAverage(0, m_ComData->layer_dataSeriesA[m_ComData->T2_Cur_Index - 1]);
                else if(CurDifferT_A > 0) {
                    for(qint64 i = m_ComData->T1_Cur_Index; i < m_ComData->T2_Cur_Index; i++)
                    {
                        sunCurrent += m_ComData->layer_dataSeriesA[i - 1];
                    }
                    if(CurDifferT_A != 0)
                        sunCurrent = sunCurrent / CurDifferT_A;
                    emit signalUpdateCurAverage(CurDifferT_A, sunCurrent);
                } else if(CurDifferT_A < 0) {
                    CurDifferT_A = 0 - CurDifferT_A;
                    for(qint64 i = m_ComData->T2_Cur_Index; i < m_ComData->T1_Cur_Index; i++)
                    {
                        sunCurrent += m_ComData->layer_dataSeriesA[i - 1];
                    }
                    if(CurDifferT_A != 0)
                        sunCurrent = sunCurrent / CurDifferT_A;
                    emit signalUpdateCurAverage(CurDifferT_A, sunCurrent);
                }

            }
        } catch (...) {
            qDebug() << "计算电流平均值异常";
        }

        try {
            if(m_ComData->T1_Vol_Index > 0 && m_ComData->T1_Vol_Index <= m_ComData->layer_currentIndex &&
                    m_ComData->T2_Vol_Index > 0 && m_ComData->T2_Vol_Index <= m_ComData->layer_currentIndex &&
                    CurDifferT_V != m_ComData->T2_Vol_Index - m_ComData->T1_Vol_Index)
            {
                CurDifferT_V = m_ComData->T2_Vol_Index - m_ComData->T1_Vol_Index;

                double sunCurrent = 0;

                if(CurDifferT_V == 0)
                    emit signalUpdateVolAverage(0, m_ComData->layer_dataSeriesV[m_ComData->T2_Vol_Index - 1]);
                else if (CurDifferT_V > 0) {
                    for(qint64 i = m_ComData->T1_Vol_Index; i < m_ComData->T2_Vol_Index; i++)
                    {
                        sunCurrent += m_ComData->layer_dataSeriesV[i - 1];
                    }
                    sunCurrent = sunCurrent / CurDifferT_V;
                    emit signalUpdateVolAverage(CurDifferT_V, sunCurrent);
                } else if (CurDifferT_V < 0) {
                    CurDifferT_V = 0 - CurDifferT_V;
                    for(qint64 i = m_ComData->T2_Vol_Index; i < m_ComData->T1_Vol_Index; i++)
                    {
                        sunCurrent += m_ComData->layer_dataSeriesV[i - 1];
                    }
                    sunCurrent = sunCurrent / CurDifferT_V;
                    emit signalUpdateVolAverage(CurDifferT_V, sunCurrent);
                }

            }
        } catch (...) {
            qDebug() << "计算电压平均值异常";
        }


        this->msleep(100);
    }
    emit end_Thread();      // 发送信号
    this->quit();
    this->wait();
}
