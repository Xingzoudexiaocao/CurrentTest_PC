#include "usb_send_thread.h"

USB_Send_Thread::USB_Send_Thread(QObject *parent, USB_HID *hid, ComData *comD) : QThread ()
{
    qDebug("发送线程create: %d", this->currentThreadId());
    m_UsbHid = hid;
    m_ComData = comD;
    isStop = true;
    this->AckState = -1;
}
USB_Send_Thread::~USB_Send_Thread()
{
    qDebug() << "USB_Send_Thread析构函数。";
}
void USB_Send_Thread::run()
{
    qDebug("发送线程run: %d", this->currentThreadId());
    qDebug() << "SendThread:length = " << m_ComData->updataFileLen;
//    for (int i = 0; i < (int)m_ComData->updataFileLen; i++) {
//        qDebug("%x ", *(m_ComData->updataFile + i));
//    }
    unsigned long upIndex = 0;
    unsigned long upStep = m_ComData->updataFileLen / 1024;

    unsigned char upVersion[4];
    unsigned long long upLength = m_ComData->updataFileLen;
    unsigned char sendPage[1056];
    unsigned char sendOnce[32];
    unsigned char pageMax = sizeof(sendPage) / sizeof(sendOnce);
    unsigned char pageIndex = 0;
    unsigned int crcValue = 0;
    int ackValue = -1;

    // 设置版本号
    memcpy(upVersion,  m_ComData->updataFile + (m_ComData->updataFileLen), sizeof (upVersion));  // 读取最后四个字节

#ifdef appUpdataDebug
    upVersion[0] = 'V';     // 调试模式版本号为V128.128.128
    memset(upVersion + 1, 0x80, sizeof (sendOnce) - 1);
#endif

    // send head data
    memset(sendOnce, 0x00, sizeof (sendOnce));
    sendOnce[0] = YMODEM_SOH; sendOnce[1] = 0x00; sendOnce[2] = 0xFF;
    memcpy(sendOnce + 3,  upVersion, 4);
    memcpy(sendOnce + 7,  &upLength, 4);
    crcValue = USB_Send_Thread::YModemCRC(sendOnce + 3, 8);
    sendOnce[11] = crcValue >> 8;
    sendOnce[12] = crcValue & 0xFF;
    m_UsbHid->SendUSB(sendOnce, 32);   // 使用USB发送数据
//    for (int i = 0; i < sizeof(sendOnce); i++) {
//        qDebug("%x ", *(sendOnce + i));
//    }
    ackValue = getAck();
    if (ackValue == 2) {
        // next
        qDebug() << "Get ACK; success! upStep = " << upStep;
    } else if (ackValue == 1) {
        qDebug("Time Out;");
        emit usbTimeOut();		// send signal: TimeOut;
        emit end_Thread();      // 发送信号
        this->quit();
        this->wait();
    } else {
        qDebug("Get Nak; fail!");
        emit usbFail();		// send signal: USB hand fail;
        emit end_Thread();      // 发送信号
        this->quit();
        this->wait();
    }
    // send file data
    upIndex = 0;
    while(upIndex < upStep)
    {
        memset(sendPage,  0x00, sizeof(sendPage));
        sendPage[0] = YMODEM_STX; sendPage[1] = upIndex + 1; sendPage[2] = ~sendPage[1];
        memcpy(sendPage + 3,  m_ComData->updataFile + (upIndex * 1024), 1024);
        crcValue = USB_Send_Thread::YModemCRC(sendPage + 3, 1024);
        sendPage[1027] = crcValue >> 8;
        sendPage[1028] = crcValue & 0xFF;
//        m_UsbHid->SendUSB(sendPage, sizeof (sendPage));   // 使用USB发送数据

        pageIndex = 0;
        qDebug() << "Send upIndex:" << upIndex << " pageMax:" << pageMax;
        while(pageIndex < pageMax)
        {
            qDebug() << "Send pageIndex:" << pageIndex << " pageMax:" << pageMax << " Time = " << QDateTime::currentDateTime();
            memcpy(sendOnce,  sendPage + (pageIndex * 32), 32);
            m_UsbHid->SendUSB(sendOnce, 32);   // 使用USB发送数据
            emit sendProgressBar(upIndex * 1024 + pageIndex * 32);		// send signal: process
            this->msleep(20);
            pageIndex++;
        }

        ackValue = getAck();
        if (ackValue == 2) {
            upIndex++;	// next
        } else if (ackValue == 1) {
            emit usbTimeOut();		// send signal: TimeOut;
        } else {
            // send again;
        }

        this->msleep(100);
    }
    // send end data
    sendOnce[0] = YMODEM_EOT; sendOnce[1] = 0x00; sendOnce[2] = 0xFF;
    memcpy(sendOnce + 3,  upVersion, 4);
    memcpy(sendOnce + 7,  &upLength, 4);
    crcValue = USB_Send_Thread::YModemCRC(sendOnce + 3, 8);
    sendOnce[11] = crcValue >> 8;
    sendOnce[12] = crcValue & 0xFF;
    m_UsbHid->SendUSB(sendOnce, 32);   // 使用USB发送数据
    ackValue = getAck();
    if (ackValue == 2) {
        // next
        emit usbSuccess();		// send singal: send File success;
    } else if (ackValue == 1) {
        emit usbTimeOut();		// send signal: TimeOut;
    } else {
        emit usbFail();		// send signal: USB hand fail;
    }

    emit end_Thread();      // 发送信号
    this->quit();
    this->wait();
//    this->terminate();
}

int USB_Send_Thread::getAck(void)
{
    int ren = 0;
    int msCnt = 0;
    while(!isStop) {
        if(this->AckState == YMODEM_ACK) {
            ren = 2;
            this->AckState = -1;
            break;
        } else if(this->AckState == YMODEM_NAK) {
            ren = 0;
            this->AckState = -1;
            break;
        } else if(this->AckState == YMODEM_TIMEOUT) {
            this->AckState = -1;
            return 1;   // TimeOut
        } else {
            this->AckState = -1;
        }
        this->msleep(1);
        msCnt++;
        if(msCnt > 1000) {
            ren = 1;
            break;
        }
    }

    return ren;		// 0=NoAck;1=TimeOut;2=ACK
}

void USB_Send_Thread::setAckState(int getAck)			// slot function
{
    this->AckState = getAck;
}
