#ifndef USB_HID_H
#define USB_HID_H
#include "comdata.h"

class USB_HID
{
public:
    USB_HID(ComData *comD);
    ~USB_HID();
public:
    hid_device *handle;     // hidapi库
    libusb_device_handle *dev_handle;         //a device handle   libusb库
    libusb_context *ctx;    //a libusb session

    char str_Manufactured[64], str_Product[64], str_SerialNumber[64];

    bool ConnectUSB();
    bool DisConnectUSB();
    int SendUSB(unsigned char *buf, unsigned char len);
    void ReadUSB();

private:
    ComData *m_ComData;
};

#endif // USB_HID_H
