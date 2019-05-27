#ifndef USB_SEND_THREAD_H
#define USB_SEND_THREAD_H

#include <QObject>

class USB_Send_Thread : public QObject
{
    Q_OBJECT
public:
    explicit USB_Send_Thread(QObject *parent = nullptr);

signals:

public slots:
};

#endif // USB_SEND_THREAD_H
