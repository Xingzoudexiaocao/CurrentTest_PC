#ifndef SQLITE_THREAD_H
#define SQLITE_THREAD_H

#include <QObject>
#include <QThread>
#include <QtDebug>
#include "comdata.h"
#include "usb_hid.h"
#include "qchartviewer.h"
//#include <QFontDatabase>
//#include <QCoreApplication>
#include <QDir>
//#include <QFileInfo>
//#include <QFileDialog>
//#include <QMessageBox>

class sqlite_thread : public QThread
{
    Q_OBJECT
public:
    explicit sqlite_thread(QObject *parent = nullptr, USB_HID *hid = nullptr, ComData *comD = nullptr);
    ~sqlite_thread();

signals:
    void emitQBoxTip(QString);
    void end_Thread();
    void show_Average();
    void show_Energy();
    void show_time(qint64 runingT, qint64 remainT);

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
    qint64 mStringCount;    // 数据库数量

public slots:
    void writeSqliteData(qint64 time, double vol, double cur);
    void CreateSqlite_T(void);
    void CreateSqliteByName(QString str);
};

#endif // SQLITE_THREAD_H
