#include "usb_receive_thread.h"
#include "usb_send_thread.h"

USB_Receive_Thread::USB_Receive_Thread(QObject *parent, USB_HID *hid, ComData *comD) : QThread ()
{
    qDebug("接收线程create: %d", this->currentThreadId());
    m_UsbHid = hid;
    m_ComData = comD;
    isStop = false;
}
USB_Receive_Thread::~USB_Receive_Thread()
{
    qDebug() << "USB_Receive_Thread析构函数。";

}
void USB_Receive_Thread::run()
{
    qDebug("接收线程run: %d", this->currentThreadId());
     qint64 testCnt_S = 0;
     qint64 testTime = 0;
    int connectCount = 0;
    int res = -1;
    int numBytes = -1;
    unsigned char buffer[32];
    memset(buffer, 0, 32);

    m_ComData->ClearData();         // 清之前的数据
    CreateRandomKey();          // 生成随机密钥

    while (!isStop) {
//        if(isStop)
//            break;
//        if(!isStop) {
            /* Wait up to 5 seconds for a message to arrive on endpoint 0x81. */
//            res = libusb_claim_interface(m_UsbHid->dev_handle, 0);            //claim interface 0 (the first) of device (mine had jsut 1)
//            if(res < 0)
//            {
//                qDebug()<<"Cannot Claim Interface"<<endl;
//                return;
//            }
//        //    qDebug()<<"Claimed Interface"<<endl;
            res = libusb_interrupt_transfer(m_UsbHid->dev_handle, 0x81, buffer, 32, &numBytes, 0);      // 100
            if (0 == res)
            {
              if (numBytes == 32)
              {
                  connectCount = 0;
                  if(buffer[0] == YMODEM_ACK && buffer[1] == YMODEM_ACK && buffer[2] == YMODEM_ACK && buffer[3] == YMODEM_ACK)
                      emit setAckOrNak(YMODEM_ACK);
                  else if(buffer[0] == YMODEM_NAK && buffer[1] == YMODEM_NAK && buffer[2] == YMODEM_NAK && buffer[3] == YMODEM_NAK)
                      emit setAckOrNak(YMODEM_NAK);
                  else if(buffer[0] == YMODEM_TIMEOUT && buffer[1] == YMODEM_TIMEOUT && buffer[2] == YMODEM_TIMEOUT && buffer[3] == YMODEM_TIMEOUT)
                      emit setAckOrNak(YMODEM_TIMEOUT);
                  else if(buffer[0] == YMODEM_VER_LEN && buffer[1] == YMODEM_VER_LEN && buffer[2] == YMODEM_VER_LEN && buffer[3] == YMODEM_VER_LEN)
                  {
                      unsigned long long ver,len;
                      memcpy(&ver,  buffer + 4, 4);   // 赋值版本号
                      memcpy(&len,  buffer + 8, 4);   // 赋值文件长度
                      emit get_Version_Length(ver, len);
                  }
                  else if(buffer[0] == YMODEM_VALID_VALUE_1 && buffer[1] == YMODEM_VALID_VALUE_1 && buffer[2] == YMODEM_VALID_VALUE_1 && buffer[3] == YMODEM_VALID_VALUE_1)
                  {
                      memcpy(&m_ComData->d_verifyValue.Lelve_2_min,  buffer + 4, 4);
                      memcpy(&m_ComData->d_verifyValue.Lelve_2_max,  buffer + 8, 4);
                      memcpy(&m_ComData->d_verifyValue.Lelve_3_min,  buffer + 12, 4);
                      memcpy(&m_ComData->d_verifyValue.Lelve_3_max,  buffer + 16, 4);
                      memcpy(&m_ComData->d_verifyValue.Lelve_4_min,  buffer + 20, 4);
                      memcpy(&m_ComData->d_verifyValue.Lelve_4_max,  buffer + 24, 4);
                      m_ComData->isGetVarify = true;
                      emit get_Verify_Value();
                  }
                  else if(buffer[0] == YMODEM_RANDOM_KEY && buffer[1] == YMODEM_RANDOM_KEY && buffer[2] == YMODEM_RANDOM_KEY && buffer[3] == YMODEM_RANDOM_KEY)
                  {
//                      quint8 getBuf[8];
                      quint16 getShort = (quint16)buffer[4] << 8 | buffer[5];
//                      memcpy(getBuf, buffer + 16, sizeof (getBuf));
//                      if(0 == memcmp(getBuf, m_ComData->key,sizeof (getBuf)))
                      if(getShort == USB_Send_Thread::YModemCRC(m_ComData->key, 8))
                      {
                          m_ComData->isKey = true;
                          emit get_RandomKey_Value();
                      }
                  }
                  else
                  {
                      if(m_ComData->isGetVarify && m_ComData->isKey)
                      {
                          ST_REC_STRUCT *tmp = new ST_REC_STRUCT();
                          for(int k = 0; k < 24; k ++)
                          {
                              buffer[4 + k] = buffer[4 + k] ^ m_ComData->key[k % 8];
                          }
                          memcpy(tmp, buffer, 32);
//                      qDebug("Received %d bytes, 成功.", numBytes);
//                      qDebug() << QDateTime::currentDateTime();
//                       m_ComData->RunningCount++;       // 时间加1ms

/*
                       qint64 testCnt = 0;
                       testCnt |= buffer[19]; testCnt = testCnt << 8;
                       testCnt |= buffer[18]; testCnt = testCnt << 8;
                       testCnt |= buffer[17]; testCnt = testCnt << 8;
                       testCnt |= buffer[16];
                       qint64 testUsbSendCnt = 0;
                       testUsbSendCnt |= buffer[23]; testUsbSendCnt = testUsbSendCnt << 8;
                       testUsbSendCnt |= buffer[22]; testUsbSendCnt = testUsbSendCnt << 8;
                       testUsbSendCnt |= buffer[21]; testUsbSendCnt = testUsbSendCnt << 8;
                       testUsbSendCnt |= buffer[20];

                       if(testCnt - testCnt_S > 1)
                       {
                           testTime++;
                           qDebug() << testTime << "__" << testCnt - testCnt_S << "testUsbSendCnt = " << testUsbSendCnt << ", testCnt =" << testCnt << " Time = " << QDateTime::currentDateTime();
                       }
                       testCnt_S = testCnt;
*/
                           HandleData(tmp);  // 处理数据
                           delete tmp;
                      }

                  }
              }
              else
              {
//                  m_ComData->testNumberRight++;
                  qDebug("Received %d bytes, 数值不对.\n", numBytes);
              }

            }
            else
            {
//                m_ComData->testNumberWrong++;
                qDebug("Error receiving message.\n");
            }

//            res = libusb_release_interface(m_UsbHid->dev_handle, 0); //release the claimed interface
//            if(res !=0)
//            {
//                qDebug()<<"Cannot Release Interface"<<endl;
//                return;
//            }
//        //    qDebug()<<"Released Interface"<<endl;
            res = -1;   // 置位
            numBytes = -1;
//            QThread::sleep(0);

            connectCount++;
            if(connectCount == 2000)
                break;

//            QThread::usleep(10);   // 延时100us
//        } else {
//            QThread::msleep(200);
//            emit end_Thread();      // 发送信号
////            this->terminate();      // 关闭线程
//            this->quit();
//            this->wait();
//            break;
//        }
    }
    emit end_Thread();      // 发送信号
    this->quit();
    this->wait();
}

void USB_Receive_Thread::HandleData(ST_REC_STRUCT *bufData)
{
    unsigned char buf[32];
    memcpy(buf, bufData, 32);
    unsigned char *getHeader = new unsigned char[4]{};
    memcpy(getHeader, buf, m_ComData->headerLength);
    if(buf[28] != 0x59 || buf[29] != 0x3E || buf[30] != 0xBD)
    {
        qDebug() << "尾码不正确"  << buf[27]  << buf[28] << buf[29] << buf[30] << buf[31];
        return;     // 数据尾码不对，返回
    }
    emit send_Level_Num(buf[27]);
    if(buf[27] == 0 || buf[27] > 4)
    {
        qDebug() << "档位不正确" << buf[27]  << buf[26] << buf[25] << buf[24] << buf[23];
        return;     // 档位不正确
    }
    if(0 == memcmp(getHeader, m_ComData->headerC, m_ComData->headerLength))
    {
        unsigned int sum_1 = buf[5] + buf[6] + buf[7] + 0x9B;
        unsigned int sum_2 = buf[15] + buf[16] + buf[17] + 0x9B;
        if((buf[4] != (sum_1 & 0xFF))||(buf[14] != (sum_2 & 0xFF)))
        {
            qDebug() << "校验和不正确" << buf[4] << buf[5] << buf[6] << buf[7];
            qDebug() << "校验和不正确" << buf[14] << buf[15] << buf[16] << buf[17];
            return;     // 校验和不正确
        }
        for (int i = 0; i < 2; i++) {
            m_ComData->RunningCount++;
            d_And_c dataB;
            d_And_c dataC;
      //      memcpy(&dataB, buf + 4, sizeof(double));
      //      memcpy(&dataC, buf + 12, sizeof(double));
            // 赋值
            unsigned int buf_Vol = 0;
            unsigned int buf_Temp = 0;
            if(i == 1) {
                buf_Vol |= buf[8]; buf_Vol = buf_Vol << 8;            // aADCxConvertedValues[0]电压
                buf_Vol |= buf[9];
                buf_Temp |= buf[10]; buf_Temp = buf_Temp << 8;        // aADCxConvertedValues[1]温度
                buf_Temp |= buf[11];
            } else if(i == 0) {
                buf_Vol |= buf[18]; buf_Vol = buf_Vol << 8;            // aADCxConvertedValues[0]电压
                buf_Vol |= buf[19];
                buf_Temp |= buf[20]; buf_Temp = buf_Temp << 8;        // aADCxConvertedValues[1]温度
                buf_Temp |= buf[21];
            }
            dataB.d = (double)buf_Vol / 4096 * 3 *2.5 ;  // 1.0043为修正值  * 1.0043
            double temperature = (1.43 - ((double)buf_Temp / 4095 * 3)) / 4.3 + 25;   // 计算温度
            unsigned int buf_Cur = 0;
            unsigned int max_Cur = 0x7FFFFF;
            double adVol = 0;     // 转换后ad采样的电压值
            if(i == 1) {
                buf_Cur |= buf[7]; buf_Cur = buf_Cur << 8;
                buf_Cur |= buf[6]; buf_Cur = buf_Cur << 8;
                buf_Cur |= buf[5];
            } else if(i == 0) {
                buf_Cur |= buf[17]; buf_Cur = buf_Cur << 8;
                buf_Cur |= buf[16]; buf_Cur = buf_Cur << 8;
                buf_Cur |= buf[15];
            }
            double stepV = 0;     // 每一个对应的电流值
            adVol = (double)buf_Cur / (double)max_Cur * 2500;     // 获取ad采样的电压值，mv为单位
            switch (buf[27]) {
              case 1:
                if((m_ComData->d_verifyValue.Lelve_1_max - m_ComData->d_verifyValue.Lelve_1_min) != 0 && m_ComData->SettingIsVerified) {
                    stepV = (m_ComData->d_standardValue.Lelve_1_max - m_ComData->d_standardValue.Lelve_1_min) / ((double)(m_ComData->d_verifyValue.Lelve_1_max - m_ComData->d_verifyValue.Lelve_1_min));
                    dataC.d = m_ComData->d_standardValue.Lelve_1_min + stepV * ((double)buf_Cur - (double)m_ComData->d_verifyValue.Lelve_1_min);
                } else {
                    stepV = 0;
                    dataC.d = adVol / 12.52 / 100000;
                }

                break;
              case 2:
                if((m_ComData->d_verifyValue.Lelve_2_max - m_ComData->d_verifyValue.Lelve_2_min) != 0  && m_ComData->SettingIsVerified) {
                    stepV = (m_ComData->d_standardValue.Lelve_2_max - m_ComData->d_standardValue.Lelve_2_min) / ((double)(m_ComData->d_verifyValue.Lelve_2_max - m_ComData->d_verifyValue.Lelve_2_min));
                    dataC.d = m_ComData->d_standardValue.Lelve_2_min + stepV * ((double)buf_Cur - (double)m_ComData->d_verifyValue.Lelve_2_min);
                } else {
                    stepV = 0;
                    dataC.d = adVol / 12.52 / 1000;
                }

                break;
              case 3:
                if((m_ComData->d_verifyValue.Lelve_3_max - m_ComData->d_verifyValue.Lelve_3_min) != 0  && m_ComData->SettingIsVerified) {
                    stepV = (m_ComData->d_standardValue.Lelve_3_max - m_ComData->d_standardValue.Lelve_3_min) / ((double)(m_ComData->d_verifyValue.Lelve_3_max - m_ComData->d_verifyValue.Lelve_3_min));
                    dataC.d = m_ComData->d_standardValue.Lelve_3_min + stepV * ((double)buf_Cur - (double)m_ComData->d_verifyValue.Lelve_3_min);
                } else {
                    stepV = 0;
        //          if((adVol / 14) >= 20) {       // 1.003 为修正mos管电压
        //              double dec = adVol / 14 - 20;
        //              dataC.d = (dec / 1.001 + 20) / 10;  // 大于3mA电流需要减去mos管电压
        //          } else {
                      dataC.d = adVol / 12.5 / 10;
        //          }
                }

                    break;
                  case 4:
                    if((m_ComData->d_verifyValue.Lelve_4_max - m_ComData->d_verifyValue.Lelve_4_min) != 0  && m_ComData->SettingIsVerified) {
                        stepV = (m_ComData->d_standardValue.Lelve_4_max - m_ComData->d_standardValue.Lelve_4_min) / ((double)(m_ComData->d_verifyValue.Lelve_4_max - m_ComData->d_verifyValue.Lelve_4_min));
                        dataC.d = m_ComData->d_standardValue.Lelve_4_min + stepV * ((double)buf_Cur - (double)m_ComData->d_verifyValue.Lelve_4_min);
                    } else {
                        stepV = 0;
            //          dataC.d = adVol / 14 / (0.1 + 0.0075);
                        dataC.d = adVol / 12.52 / 0.1;
                    }

                    break;
                  default: dataC.d = 0; break;
                }
                if(dataC.d < 0)
                    dataC.d = 0;

            // 更新数据到表格数据
            // The current time
      //      QDateTime now = QDateTime::currentDateTime();
            QDateTime now = QDateTime::fromMSecsSinceEpoch(m_ComData->BeginTime +  m_ComData->RunningCount);
            m_ComData->lastTime = now;      // 更新接收时间
            // We need the currentTime in millisecond resolution
            double currentTime = Chart::chartTime2(now.toTime_t())
                                 + ((double)now.time().msec() / 10) * 0.01;     //     / 10 * 0.01
            // After obtaining the new values, we need to update the data arrays.
            double tmp_V = round(dataB.d * 1000) / 1000;          // 对数据进行最小精度的四舍五入
      //      double tmp_V = round(adVol) / 1000;          // 对数据进行最小精度的四舍五入，测试ad1259的采样值
            double tmp_A = round(dataC.d * 1000000) / 1000000;
            if (m_ComData->d_currentIndex < m_ComData->DataSize)
            {
                // Store the new values in the current index position, and increment the index.
                m_ComData->d_dataSeriesV[m_ComData->d_currentIndex] = tmp_V;
                m_ComData->d_dataSeriesA[m_ComData->d_currentIndex] = tmp_A;
                m_ComData->d_timeStamps[m_ComData->d_currentIndex] = currentTime;
                ++m_ComData->d_currentIndex;

            }
            else
            {
                // The data arrays are full. Shift the arrays and store the values at the end.
                ComData::shiftData_D(m_ComData->d_dataSeriesV, m_ComData->DataSize, tmp_V);
                ComData::shiftData_D(m_ComData->d_dataSeriesA, m_ComData->DataSize, tmp_A);
                ComData::shiftData_D(m_ComData->d_timeStamps, m_ComData->DataSize, currentTime);
            }
            if(!m_ComData->layerIsPause)
            {
                m_ComData->layer_currentIndex = m_ComData->d_currentIndex;
                m_ComData->layer_BeginTime = now.toMSecsSinceEpoch() - m_ComData->d_currentIndex;
//                memcpy(m_ComData->layer_timeStamps, m_ComData->d_timeStamps, sizeof(double) * m_ComData->d_currentIndex);
//                memcpy(m_ComData->layer_dataSeriesV, m_ComData->d_dataSeriesV, sizeof(double) * m_ComData->d_currentIndex);
//                memcpy(m_ComData->layer_dataSeriesA, m_ComData->d_dataSeriesA, sizeof(double) * m_ComData->d_currentIndex);
            }
            emit get_Vol_Cur_Now(now.toMSecsSinceEpoch(), dataB.d, dataC.d);
      //      qDebug() << "--单次接收的数据：dataB = " << dataB.d << ", dataC =" << dataC.d  << " Time = " << QDateTime::currentDateTime();
      //      qDebug() << "单次接收数据：" <<  QDateTime::fromMSecsSinceEpoch(m_ComData->d_timeStamps[m_ComData->d_currentIndex - 1]) << QDateTime::currentDateTime();
            // 处理电流过大/过小的情况
            unsigned char tips = 0;
            if(buf[26] == 0x01)
            {
                tips = 1;
            }
            else if(buf[26] == 0x02)
            {
                tips = 2;
            }
            emit get_USB_Data(now, temperature, tips, 0);    // 发送接收信号
        }


    } else {
        qDebug() << "头码不正确" << buf[0] << buf[1] << buf[2] << buf[3];
        // 处理错误代码
        if(buf[0] == 0xaa && buf[1] == 0xbb && buf[2] == 0xcc && buf[3] == 0xdd)
        {
            QDateTime now = QDateTime::currentDateTime();
            unsigned int buf_Cur = 0;
            buf_Cur |= buf[7]; buf_Cur = buf_Cur << 8;
            buf_Cur |= buf[6]; buf_Cur = buf_Cur << 8;
            buf_Cur |= buf[5]; buf_Cur = buf_Cur << 8; buf_Cur |= buf[4];
            if(buf_Cur >= 0x80000000)
            {
                emit get_USB_Data(now, 0, 0, (unsigned char)(buf_Cur - 0x80000000));    // 发送接收信号
            }
            qDebug() << now << "错误代码cmd = " << (unsigned char)(buf_Cur - 0x80000000) << buf[7] << buf[6] << buf[5] << buf[4] ;
        }
    }

    delete []getHeader;
}

void USB_Receive_Thread::CreateRandomKey(void)
{
    qint16 random1 = qrand();
    qint16 random2 = qrand();
    qint16 random3 = qrand();
    qint16 random4 = qrand();
    m_ComData->key[0] = random1 & 0xFF;  m_ComData->key[1] = (random1 >> 8) & 0xFF;
    m_ComData->key[2] = random2 & 0xFF;  m_ComData->key[3] = (random2 >> 8) & 0xFF;
    m_ComData->key[4] = random3 & 0xFF;  m_ComData->key[5] = (random3 >> 8) & 0xFF;
    m_ComData->key[6] = random4 & 0xFF;  m_ComData->key[7] = (random4 >> 8) & 0xFF;

//    qDebug() << "当前时间" << QTime::currentTime();
//    quint8 m_text[8];
//    quint8 e_text[8];
//    memset(m_text, 0x88, sizeof (m_text));
//    memset(e_text, 0x00, sizeof (e_text));
//    endes(m_text, m_ComData->key, e_text);
//    memset(m_text, 0x00, sizeof (m_text));
//    undes(e_text, m_ComData->key, m_text);
//    qDebug() << "当前时间" << QTime::currentTime();
//    qDebug() << "e_text" << e_text[0] << e_text[1] << e_text[2] << e_text[3] << e_text[4] << e_text[5] << e_text[6] << e_text[7];
//    qDebug() << "m_text" << m_text[0] << m_text[1] << m_text[2] << m_text[3] << m_text[4] << m_text[5] << m_text[6] << m_text[7];

}
