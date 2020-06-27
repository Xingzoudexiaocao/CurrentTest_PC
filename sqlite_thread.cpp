#include "sqlite_thread.h"

sqlite_thread::sqlite_thread(QObject *parent, USB_HID *hid, ComData *comD) : QThread ()
{
    qDebug("数据库操作create: %d", this->currentThreadId());
    m_UsbHid = hid;
    m_ComData = comD;
    isStop = false;
    m_DbName = "";        // 清空数据文件名称
}
sqlite_thread::~sqlite_thread()
{
    qDebug() << "sqlite_thread析构函数。";
}
void sqlite_thread::run()
{
    qDebug("数据库操作run: %d", this->currentThreadId());
    mDbCount = 0;

    m_DbName = QDir::currentPath() + "/iSCAN_Data/" + QDateTime::currentDateTime().toString("yyyy_MM_dd hh_mm_ss") + " Record.db";
    CreateSqliteByName(m_DbName);   // 创建文件
    while (!isStop) {
        // 写数据库
        if(m_DbData.size() >= 10000)        // 每次写入10000个数据到数据库中
        {
            QString ConnectName;
            {
                QSqlDatabase db;
                if(QSqlDatabase::contains("CurrentData"))
                  db = QSqlDatabase::addDatabase("QSQLITE", "CurrentData");
                else
                  db = QSqlDatabase::addDatabase("QSQLITE");

                db.setDatabaseName(m_DbName);    // QApplication::applicationDirPath() + "CONFIG.db"     不能包含字符
           //     db.setUserName("admin");
           //     db.setPassword("admin");
                if (!db.open())     // if (!db.open("admin","admin"))
                {
                    qDebug() << "创建数据库文件失败！";
                    emit emitQBoxTip("创建数据库文件失败！");
    //                QMessageBox::critical(this, "提示", "创建数据库文件失败！");
                    return;
                }
                QSqlQuery query("", db);
                 // 开始启动事务
        //              qDebug() << "插入数据前时间" << QDateTime::currentDateTime();
        //              qDebug() << "m_DbData List" << m_DbData.size();
        //         bool    bsuccess = false;
        //         QTime    tmpTime;
                  db.transaction();
        //         tmpTime.start();
                  query.prepare("INSERT INTO stm32_data (time, voltage, current) "
                                    "VALUES (:time, :voltage, :current)");   //为每一列标题添加绑定值
//                  for(int i = 0 ; i < m_DbData.size(); i++)       //从names表里获取每个名字
//                  {
//                      query.bindValue(":time", m_DbData.at(i).time);                      //向绑定值里加入
//                      query.bindValue(":voltage", m_DbData.at(i).vol);      //
//                      query.bindValue(":current", m_DbData.at(i).cur);    //
//                      query.exec();               //加入库中
//                   }
                    while(m_DbData.size())
                    {
                        query.bindValue(":time", m_DbData.at(0).time);                      //向绑定值里加入
                        query.bindValue(":voltage", m_DbData.at(0).vol);      //
                        query.bindValue(":current", m_DbData.at(0).cur);    //
                        query.exec();               //加入库中
                        m_DbData.removeAt(0);
                    }
                 // 提交事务，这个时候才是真正打开文件执行SQL语句的时候
                 db.commit();
//                 m_DbData.clear();       // 清空所有值
                 query.clear();
                 ConnectName = QSqlDatabase::database().connectionName();
                 db.close();
             }
             QSqlDatabase::removeDatabase(ConnectName);
             mDbCount++;
             if(mDbCount >= 6 * 60 - 6)    // 1个小时重新生成数据库文件
             {
                 mDbCount = 0;
                 m_DbName = QDir::currentPath() + "/iSCAN_Data/" + QDateTime::currentDateTime().toString("yyyy_MM_dd hh_mm_ss") + " Record.db";
                 CreateSqliteByName(m_DbName);   // 创建文件
             }
        }
        this->msleep(1);
    }
    emit end_Thread();      // 发送信号
    this->quit();
    this->wait();
}

void sqlite_thread::CreateSqliteByName(QString str)
{
    // 创建数据库文件
    QDir dir(QDir::currentPath() + "/iSCAN_Data");
    if(!dir.exists())
    {
       qDebug() << "创建文件夹";
       dir.mkdir(QDir::currentPath() + "/iSCAN_Data");  //只创建一级子目录，即必须保证上级目录存在
    }
    QString ConnectName;
    {
        QSqlDatabase db;
        if(QSqlDatabase::contains("CurrentData"))
          db = QSqlDatabase::addDatabase("QSQLITE", "CurrentData");
        else
          db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(str);    // QApplication::applicationDirPath() + "CONFIG.db"     不能包含字符
    //     db.setUserName("admin");
    //     db.setPassword("admin");
        if (!db.open())     // if (!db.open("admin","admin"))
        {
            qDebug() << "创建数据库文件失败！";
            emit emitQBoxTip("创建数据库文件失败！");
    //        QMessageBox::critical(this, "提示", "创建数据库文件失败！");
            return;
        }
        else
            qDebug() << "创建数据库文件成功！" << QDateTime::currentDateTime();
        QSqlQuery query("", db);
//         if(!query.exec("select count(*)  from sqlite_master where type='table' and name = 'stm32_data'"))
//         {
//             query.exec("DROP TABLE stm32_data");        //先清空一下表
            query.exec("CREATE TABLE stm32_data ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "time INT64 NOT NULL, "
                               "voltage DOUBLE NOT NULL, "
                               "current DOUBLE NOT NULL)");         //创建一个stm32_data表
//            qDebug() << "新建表stm32_data";
//         }

        m_DbData.clear();       // 清空所有值
        query.clear();
        ConnectName = QSqlDatabase::database().connectionName();
        db.close();
    }
    QSqlDatabase::removeDatabase(ConnectName);
}

void sqlite_thread::CreateSqlite_T(void)
{
//    qDebug("测试数据", this->currentThreadId());
}

void sqlite_thread::writeSqliteData(qint64 time, double vol, double cur)
{
    static long long s_Count = 0;
//    qDebug("测试数据", this->currentThreadId());
    DB_WRITE_STRUCT buf;
    buf.time = time; buf.vol = vol; buf.cur = cur;          // QDateTime::currentDateTime().toMSecsSinceEpoch()
    m_DbData.append(buf);

    m_ComData->d_calculateValue.currentSumSecond += cur;         // mA
    m_ComData->d_calculateValue.voltageSum += vol;  m_ComData->d_calculateValue.currentSum += cur;
    s_Count++;
    if(s_Count >= 1000 * m_ComData->SettingAverageTime)
    {
        s_Count = 0;
        // 计算消耗的能量
        m_ComData->d_calculateValue.energySum += ((m_ComData->d_calculateValue.currentSumSecond / 1000) / 3600);
        m_ComData->d_calculateValue.currentSumSecond = 0;
        if(m_ComData->d_calculateValue.energySum >= m_ComData->SettingBatteryCapacity)
            m_ComData->d_calculateValue.energySum = m_ComData->SettingBatteryCapacity;
        emit show_Energy();
        // 更新平均值UI
        m_ComData->d_Avg_V = m_ComData->d_calculateValue.voltageSum / m_ComData->RunningCount;
        m_ComData->d_Avg_A = m_ComData->d_calculateValue.currentSum / m_ComData->RunningCount;
        emit show_Average();

        // 更新电池信息UI
//         qDebug() << "m_ComData->RunningCount" << m_ComData->RunningCount;
        qint64 runningMinute =  m_ComData->RunningCount / 60000;
        qint64 remainMinute = 0;
        remainMinute = (m_ComData->SettingBatteryCapacity - m_ComData->d_calculateValue.energySum) / m_ComData->d_Avg_A * 60;
        emit show_time(runningMinute, remainMinute);

    }
}
