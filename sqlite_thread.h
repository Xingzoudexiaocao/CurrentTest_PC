#ifndef SQLITE_THREAD_H
#define SQLITE_THREAD_H

#include <QObject>
#include <QThread>
#include <QtDebug>
#include "comdata.h"
#include "usb_hid.h"
#include "qchartviewer.h"
#include <QFontDatabase>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

class sqlite_thread : public QThread
{
    Q_OBJECT
public:
    explicit sqlite_thread(QObject *parent = nullptr, USB_HID *hid = nullptr, ComData *comD = nullptr);

signals:
    void emitQBoxTip(QString);

protected:
    void run();

public:
    bool isStop;

private:
    ComData *m_ComData;
    USB_HID *m_UsbHid;

    QString m_DbName;
    QList<DB_WRITE_STRUCT> m_DbData;
    qint64 mDbCount;        // 数据库存储次数计数值

public slots:
    void writeSqliteData(qint64 time, double vol, double cur);
    void CreateSqlite_T(void);
};

#endif // SQLITE_THREAD_H
