#include "usb_hid.h"
#include <QDebug>

using namespace std;

USB_HID::USB_HID(ComData *comD)
{
    m_ComData = comD;
    handle = nullptr;
    dev_handle = nullptr;
    ctx = nullptr;
}
USB_HID::~USB_HID()
{
    qDebug() << "USB_HID析构函数。";
    DisConnectUSB();
}
bool USB_HID::ConnectUSB()
{
    // Open the device using the VID, PID,

    // and optionally the Serial number.

//    handle = hid_open(0x0483, 0x5750, NULL);
//    if(handle != nullptr)
//        return true;
//     hid_device_info *devs;
//     devs = hid_enumerate(0x0, 0x0);
    /* LIBUSB 驱动打开设备，这里写的是伪代码，不保证代码有用 */

    libusb_device **devs;          //pointer to pointer of device, used to retrieve a list of devices
    int r;                         //for return values
    ssize_t cnt;                   //holding number of devices in list
    r = libusb_init(&ctx);         //initialize a library session
    if(r < 0)
    {
        qDebug() << "Init Error " << r; //there was an error
        return false;
    }
    libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if(cnt < 0)
    {
        qDebug()<<"Get Device Error"<<endl;   //there was an error
    }

    dev_handle = libusb_open_device_with_vid_pid(ctx, 0x63B2, 0xA753); //open mouse 0x0483 0x5750
    if(dev_handle == nullptr)
    {
        qDebug()<<"Cannot open device"<<endl;
        libusb_free_device_list(devs, 1); //free the list, unref the devices in it
        libusb_exit(ctx);                 //close the session
        return false;
    }
    else
    {
        libusb_device *dev_device = libusb_get_device(dev_handle);
        libusb_config_descriptor *config;
        libusb_device_descriptor desc;
        if(libusb_get_config_descriptor(dev_device, 0, &config) == 0)
        {

//            qDebug()<<"libusb_config_descriptor 设备信息：";
//            qDebug()<<"bLength = "<< config->bLength;
//            qDebug()<<"bDescriptorType = "<< config->bDescriptorType;
//            qDebug()<<"wTotalLength = "<< config->wTotalLength;
//            qDebug()<<"bNumInterfaces = "<< config->bNumInterfaces;
//            qDebug()<<"bConfigurationValue = "<< config->bConfigurationValue;
//            qDebug()<<"iConfiguration = "<< config->iConfiguration;
//            qDebug()<<"bmAttributes = "<< config->bmAttributes;
//            qDebug()<<"MaxPower = "<< config->MaxPower;
//            qDebug()<<"extra_length = "<< config->extra_length;
        }
        if (libusb_get_device_descriptor(dev_device, &desc) == 0)
        {
//            qDebug("Device Descriptors: ");
//            qDebug("Vendor ID : 0x%x",desc.idVendor);
//            qDebug("Product ID : 0x%x",desc.idProduct);
//            qDebug("Serial Number : %x",desc.iSerialNumber);
//            qDebug("Size of Device Descriptor : %d",desc.bLength);
//            qDebug("Type of Descriptor : %d",desc.bDescriptorType);
//            qDebug("USB Specification Release Number : 0x%x",desc.bcdUSB);
//            qDebug("Device Release Number : %d",desc.bcdDevice);
//            qDebug("Device Class : 0x%x",desc.bDeviceClass);
//            qDebug("Device Sub-Class : %d",desc.bDeviceSubClass);
//            qDebug("Device Protocol : %d",desc.bDeviceProtocol);
//            qDebug("Max. Packet Size : %d",desc.bMaxPacketSize0);
//            qDebug("No. of Configuraions : %d",desc.bNumConfigurations);
        }
        if (libusb_get_string_descriptor_ascii(dev_handle, desc.iManufacturer, (unsigned char*) str_Manufactured, sizeof(str_Manufactured)) >= 0)
        {
            qDebug("Manufactured : %s",str_Manufactured);
        }
        if(libusb_get_string_descriptor_ascii(dev_handle, desc.iProduct, (unsigned char*) str_Product, sizeof(str_Product)) >= 0)
        {
            qDebug("Product : %s",str_Product);
        }
        if(libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, (unsigned char*) str_SerialNumber, sizeof(str_SerialNumber)) >= 0)
        {
            qDebug("SerialNumber : %s",str_SerialNumber);
        }

        qDebug()<<"Device Opened"<<endl;
        libusb_free_device_list(devs, 1);                     //free the list, unref the devices in it

        if(libusb_kernel_driver_active(dev_handle, 0) == 1)
        { //find out if kernel driver is attached
            qDebug()<<"Kernel Driver Active"<<endl;
            if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
                qDebug()<<"Kernel Driver Detached!"<<endl;
        }

        r = libusb_claim_interface(dev_handle, 0);            //claim interface 0 (the first) of device (mine had jsut 1)
        if(r < 0)
        {
            qDebug()<<"Cannot Claim Interface"<<endl;
            return false;
        }
        qDebug()<<"Claimed Interface"<<endl;

//        int size;
//        unsigned char datain[1024]="\0";
//        for(int i=0;i<300;i++)
//        {
//            int rr = libusb_interrupt_transfer(dev_handle, 0x81, datain, 0x0004, &size,	1000);
//            qDebug()<<"libusb_interrupt_transfer rr: "<<rr<<endl;
//            qDebug()<<"size: "<<size<<endl;
//            qDebug()<<"data: ";
//            for(int j=0; j<size; j++)
//                qDebug("%02x ", (unsigned char)(datain[j]));
//            qDebug()<<"\n";
//        }

//        r = libusb_release_interface(dev_handle, 0); //release the claimed interface
//        if(r!=0)
//        {
//            qDebug()<<"Cannot Release Interface"<<endl;
//            return false;
//        }
//        qDebug()<<"Released Interface"<<endl;


        return true;
    }


    return false;

}

bool USB_HID::DisConnectUSB()
{
    if(dev_handle != nullptr)
    {
        int r = -1;
        r = libusb_release_interface(dev_handle, 0); //release the claimed interface
        if(r!=0)
        {
            qDebug()<<"Cannot Release Interface"<<endl;
            return false;
        }
        qDebug()<<"Released Interface"<<endl;
        libusb_attach_kernel_driver(dev_handle, 0);
        libusb_close(dev_handle);
        libusb_exit(ctx); //close the session
        dev_handle = nullptr;
        ctx = nullptr;
        return  true;
    }
    return false;
}

int USB_HID::SendUSB(unsigned char *buf, unsigned char len)
{
    int res                      = -1;  /* return codes from libusb functions */
    int numBytes                 = -1;  /* Actual bytes transferred. */

//    res = libusb_claim_interface(dev_handle, 0);            //claim interface 0 (the first) of device (mine had jsut 1)
//    if(res < 0)
//    {
//        qDebug()<<"Cannot Claim Interface"<<endl;
//        return -1;
//    }
//    qDebug()<<"Claimed Interface"<<endl;

    /* Send the message to endpoint 1 with a 100ms timeout. */
    res = libusb_interrupt_transfer(dev_handle, 1, buf, len, &numBytes, 100);
    if (res == 0)
    {
//      qDebug("%d bytes transmitted successfully.\n", numBytes);
    }
    else
    {
      qDebug("Error sending message to device.\n");
    }

//    res = libusb_release_interface(dev_handle, 0); //release the claimed interface
//    if(res !=0)
//    {
//        qDebug()<<"Cannot Release Interface"<<endl;
//        return -1;
//    }
//    qDebug()<<"Released Interface"<<endl;

    return numBytes;
}

void USB_HID::ReadUSB()
{
    int res                      = -1;  /* return codes from libusb functions */
    int numBytes                 = -1;  /* Actual bytes transferred. */
    unsigned char buffer[32];
    memset(buffer, 0, 32);

//    res = libusb_claim_interface(dev_handle, 0);            //claim interface 0 (the first) of device (mine had jsut 1)
//    if(res < 0)
//    {
//        qDebug()<<"Cannot Claim Interface"<<endl;
//        return;
//    }
//    qDebug()<<"Claimed Interface"<<endl;

    /* Wait up to 5 seconds for a message to arrive on endpoint 0x81. */
    res = libusb_interrupt_transfer(dev_handle, 0x81, buffer, 32, &numBytes, 100);
    if (0 == res)
    {
      if (numBytes == 32)
      {
          for (unsigned char i = 0; i < 32; i++) {
              qDebug("%d %x", i, buffer[i]);
          }
      }
      else
      {
          qDebug("Received %d bytes, expected 64.\n", numBytes);
      }
    }
    else
    {
        qDebug("Error receiving message.\n");
    }

//    res = libusb_release_interface(dev_handle, 0); //release the claimed interface
//    if(res !=0)
//    {
//        qDebug()<<"Cannot Release Interface"<<endl;
//        return;
//    }
//    qDebug()<<"Released Interface"<<endl;
}
