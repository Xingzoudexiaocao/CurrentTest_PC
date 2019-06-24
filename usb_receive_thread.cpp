#include "usb_receive_thread.h"

USB_Receive_Thread::USB_Receive_Thread(QObject *parent, USB_HID *hid, ComData *comD) : QObject(parent), QThread ()
{
    qDebug("接收线程create: %d", this->currentThreadId());
    m_UsbHid = hid;
    m_ComData = comD;
    isStop = false;
}

void USB_Receive_Thread::run()
{
    qDebug("接收线程run: %d", this->currentThreadId());
    int res = -1;
    int numBytes = -1;
    unsigned char buffer[32];
    memset(buffer, 0, 32);

    while (1) {
//        if(isStop)
//            break;
        if(!isStop) {
            /* Wait up to 5 seconds for a message to arrive on endpoint 0x81. */
            res = libusb_interrupt_transfer(m_UsbHid->dev_handle, 0x81, buffer, 32, &numBytes, 100);
            if (0 == res)
            {
              if (numBytes == 32)
              {
                  ST_REC_STRUCT *tmp = new ST_REC_STRUCT();
                  memcpy(tmp, buffer, 32);
                  emit get_USB_Data(tmp);    // 发送接收信号
                  qDebug("Received %d bytes, 成功.\n", numBytes);
              }
              else
              {
                  qDebug("Received %d bytes, 数值不对.\n", numBytes);
              }
            }
            else
            {
//                qDebug("Error receiving message.\n");
            }
            res = -1;   // 置位
            numBytes = -1;
            QThread::usleep(100);   // 延时100us
        } else {
            QThread::msleep(200);
            emit end_Thread();      // 发送信号
//            this->terminate();      // 关闭线程
            this->quit();
            this->wait();
            break;
        }
    }
}


