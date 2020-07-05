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
    qint64 CurDifferT = 0;
    while (!isStop) {

        try {
            if(m_ComData->T1_Cur_Index > 0 && m_ComData->T1_Cur_Index <= m_ComData->RunningCount &&
                    m_ComData->T2_Cur_Index > 0 && m_ComData->T2_Cur_Index <= m_ComData->RunningCount &&
                    CurDifferT != m_ComData->T2_Cur_Index - m_ComData->T1_Cur_Index)
            {
                CurDifferT = m_ComData->T2_Cur_Index - m_ComData->T1_Cur_Index;

                double sunCurrent = 0;

                if(CurDifferT == 0)
                    emit signalUpdateCurAverage(0, m_ComData->d_dataSeriesA[m_ComData->T2_Cur_Index - 1]);
                else if(CurDifferT > 0) {
                    for(qint64 i = m_ComData->T1_Cur_Index; i < m_ComData->T2_Cur_Index; i++)
                    {
                        sunCurrent += m_ComData->d_dataSeriesA[i - 1];
                    }
                    if(CurDifferT != 0)
                        sunCurrent = sunCurrent / CurDifferT;
                    emit signalUpdateCurAverage(CurDifferT, sunCurrent);
                } else if(CurDifferT < 0) {
                    CurDifferT = 0 - CurDifferT;
                    for(qint64 i = m_ComData->T2_Cur_Index; i < m_ComData->T1_Cur_Index; i++)
                    {
                        sunCurrent += m_ComData->d_dataSeriesA[i - 1];
                    }
                    if(CurDifferT != 0)
                        sunCurrent = sunCurrent / CurDifferT;
                    emit signalUpdateCurAverage(CurDifferT, sunCurrent);
                }

            }
        } catch (...) {
            qDebug() << "计算电流平均值异常";
        }

        try {
            if(m_ComData->T1_Vol_Index > 0 && m_ComData->T1_Vol_Index <= m_ComData->RunningCount &&
                    m_ComData->T2_Vol_Index > 0 && m_ComData->T2_Vol_Index <= m_ComData->RunningCount &&
                    CurDifferT != m_ComData->T2_Vol_Index - m_ComData->T1_Vol_Index)
            {
                CurDifferT = m_ComData->T2_Vol_Index - m_ComData->T1_Vol_Index;

                double sunCurrent = 0;

                if(CurDifferT == 0)
                    emit signalUpdateVolAverage(0, m_ComData->d_dataSeriesV[m_ComData->T2_Vol_Index - 1]);
                else if (CurDifferT > 0) {
                    for(qint64 i = m_ComData->T1_Vol_Index; i < m_ComData->T2_Vol_Index; i++)
                    {
                        sunCurrent += m_ComData->d_dataSeriesV[i - 1];
                    }
                    sunCurrent = sunCurrent / CurDifferT;
                    emit signalUpdateVolAverage(CurDifferT, sunCurrent);
                } else if (CurDifferT < 0) {
                    CurDifferT = 0 - CurDifferT;
                    for(qint64 i = m_ComData->T2_Vol_Index; i < m_ComData->T1_Vol_Index; i++)
                    {
                        sunCurrent += m_ComData->d_dataSeriesV[i - 1];
                    }
                    sunCurrent = sunCurrent / CurDifferT;
                    emit signalUpdateVolAverage(CurDifferT, sunCurrent);
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
