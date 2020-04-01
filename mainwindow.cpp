#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent, ComData *comD, USB_HID *hid) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "MainWindow构造函数。";
    m_ComData = comD;
    m_UsbHid = hid;
    m_Debug = nullptr;
//    QTextCodec *codec = QTextCodec::codecForName("GBK");//情况1
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
//    QTextCodec::setCodecForTr(codec);
//    QTextCodec::setCodecForLocale(codec);
//    QTextCodec::setCodecForCStrings(codec);

    setFixedSize(1680, 950);            // 1100, 885
    setGeometry(100, 50, 1680, 950);      // 设定窗口显示坐标
//    setGeometry(800, 900, 1100, 885);      // 设定窗口显示坐标
    setWindowTitle(productName);
    this->grabKeyboard();       // 捕获键盘输入

    demo = new RealTime(this, m_ComData, m_UsbHid);
//    demo->setStyleSheet("* {font-family:arial;font-size:15px}");
//    demo->setGeometry(8, 20, 990, 850);
    demo->show();

//#ifdef  isDebuf
//    OpenDebugWindow();  // 默认开启
//#endif

//    QObject::connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(on_ClearData_clicked()));
//    QObject::connect(ui->pushButton_2,SIGNAL(clicked()), this, SLOT(on_SendData_clicked()));
//    QObject::connect(ui->pushButton_3,SIGNAL(clicked()), this, SLOT(on_SearchCom_clicked()));
//    QObject::connect(ui->pushButton_4,SIGNAL(clicked()), this, SLOT(on_OpenCom_clicked()));
//    QObject::connect(ui->pushButton_5,SIGNAL(clicked()), this, SLOT(on_AutoSend_clicked()));
//    QObject::connect(&serial, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);
//    QStringList boxList;//停止位
//    //发送按键失能
//    ui->pushButton_2->setEnabled(false);
//    ui->pushButton_5->setEnabled(false);
//    //波特率默认选择下拉第三项：9600
//    boxList.clear();
//    boxList <<tr("1200")<<tr("2400")<<tr("4800")<<tr("9600")<<tr("115200")<<tr("256000");
//    ui->comboBox_2->addItems(boxList);
//    ui->comboBox_2->setCurrentIndex(4);
//    //数据位默认显示8bit
//    boxList.clear();
//    boxList <<tr("8bit")<<tr("7bit")<<tr("6bit")<<tr("5bit");
//    ui->comboBox_3->addItems(boxList);
//    ui->comboBox_3->setCurrentIndex(0);
//    // 校验位默认显示 无校验
//    boxList.clear();
//    boxList <<tr("None 无")<<tr("Odd 奇")<<tr("Even 偶")<<tr("Mark 1")<<tr("Space 0");
//    ui->comboBox_4->addItems(boxList);
//    ui->comboBox_4->setCurrentIndex(0);
//    // 停止位默认为 1bit
//    boxList.clear();
//    boxList <<tr("1bit")<<tr("1.5bit")<<tr("2bit");
//    ui->comboBox_5->addItems(boxList);
//    ui->comboBox_5->setCurrentIndex(0);

    // 初始化接收缓存
    RecIndex = 0;
    for (unsigned long long i = 0; i < 1024; i++) {
        RecBuffer[i] = 0;
    }

}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow析构函数。";
    delete ui;
    delete m_ComData;
    delete m_UsbHid;
}

void MainWindow::colseDebug()
{
//    qDebug() << "Debug关闭。";
    m_Debug = nullptr;
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_P) {
        qDebug() << "Ctrl+p 按键按下。";
#ifdef  isDebuf
    OpenDebugWindow();
#endif
        return;
    }
    QWidget::keyPressEvent(ev);
}
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    if(ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_P) {
        qDebug() << "Ctrl+p 按键松开。";
        return;
    }
    QWidget::keyReleaseEvent(ev);
}

void MainWindow::OpenDebugWindow()
{
    if(m_Debug == nullptr)
    {
        m_Debug = new DebugWatch(nullptr, m_ComData, m_UsbHid);
//        demo->setStyleSheet("* {font-family:arial;font-size:15px}");
//        demo->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // 置顶，最小化，关闭
        m_Debug->setGeometry(0, 0, 400, 600);
        m_Debug->show();
        connect(demo->m_UsbReceiveThread,SIGNAL(send_Level_Num(int)),m_Debug, SLOT(receive_Level_Num(int)));
        connect(demo->m_UsbReceiveThread,SIGNAL(get_Verify_Value()),m_Debug, SLOT(receive_Verify_Value()));
        connect(m_Debug,SIGNAL(destroyed()),this, SLOT(colseDebug()));
    }
}

void MainWindow::serialPort_readyRead()
{
    //从接收缓冲区中读取数据
    QByteArray buffer = serial.readAll();
//    qDebug() << "单次接收到数据长度" << buffer.length();
    if(buffer.length() > 1024)
        return;
    for (int i = 0; i <  buffer.length(); i++) {    // 缓存数据
        if(RecIndex < 1024)
        {
            RecBuffer[RecIndex] = buffer.data()[i];
            ++RecIndex;
        }
        else
        {
            ComData::shiftData_C(RecBuffer, 1024, buffer.data()[i]);
        }
    }
    // 解析数据
    unsigned long long iIndex = 0;
    for (unsigned long long i = 0; i <  1024 - 20; i++)
    {
        if(RecIndex >= 20)
        {
            unsigned char *getHeader = new unsigned char[4]{};
            memcpy(getHeader, RecBuffer + i, m_ComData->headerLength);
            if(0 == memcmp(getHeader, m_ComData->headerC, m_ComData->headerLength))
            {
                d_And_c dataB;
                d_And_c dataC;
                memcpy(&dataB, RecBuffer + i + 4, sizeof(double));
                memcpy(&dataC, RecBuffer + i + 12, sizeof(double));
                for (unsigned long long j = 0; j < 20; j++)
                    RecBuffer[i + j] = 0;  // 清空接收区
                RecIndex = RecIndex - 20;   //
                ++iIndex;   // 统计解析了多少次数据
                // 更新数据到表格数据
                // The current time
                QDateTime now = QDateTime::currentDateTime();
                m_ComData->lastTime = now;      // 更新接收时间
                // We need the currentTime in millisecond resolution
                double currentTime = Chart::chartTime2(now.toTime_t())
                                     + now.time().msec() / 10 * 0.01;
                // After obtaining the new values, we need to update the data arrays.
                if (m_ComData->d_currentIndex < m_ComData->DataSize)
                {
                    // Store the new values in the current index position, and increment the index.
                    m_ComData->d_dataSeriesV[m_ComData->d_currentIndex] = dataB.d;
                    m_ComData->d_dataSeriesA[m_ComData->d_currentIndex] = dataC.d;
                    m_ComData->d_timeStamps[m_ComData->d_currentIndex] = currentTime;
                    ++m_ComData->d_currentIndex;
                }
                else
                {
                    // The data arrays are full. Shift the arrays and store the values at the end.
                    ComData::shiftData_D(m_ComData->d_dataSeriesV, m_ComData->DataSize, dataB.d);
                    ComData::shiftData_D(m_ComData->d_dataSeriesA, m_ComData->DataSize, dataC.d);
                    ComData::shiftData_D(m_ComData->d_timeStamps, m_ComData->DataSize, currentTime);
                }
                qDebug() << "--单次接收的数据：dataB = " << dataB.d << ", dataC =" << dataC.d  << " Time = " << QDateTime::currentDateTime();
            }
    //        else
    //        {
    //            qDebug() << "数据解析错误（头码有误），丢弃" << buffer.length() << "个数据。";
    //            qDebug("错误头码：%#x %#x %#x %#x", (unsigned char)buffer.data()[0], (unsigned char)buffer.data()[1], (unsigned char)buffer.data()[2], (unsigned char)buffer.data()[3]);
    //        }

        }
    }
    if(iIndex > 0)
    {
        for (unsigned long long i = 20 * iIndex; i <  1024; i++)
        {
            RecBuffer[i - 20 * iIndex] = RecBuffer[i];  // 将后面没有解析到的数据向前移
            RecBuffer[i] = 0;
        }
    }
    qDebug() << "当前缓存位置：" << RecIndex;
    if(RecIndex > 500)
    {
        qDebug() << "错误数据多于25次。" << RecIndex;
        RecIndex = 0;
    }

//    //从界面中读取以前收到的数据
//    QString recv = ui->textEdit->toPlainText();
//    recv += QString(buffer);
//    //清空以前的显示
//    ui->textEdit->clear();
//    //重新显示
//    ui->textEdit->append(recv);

}

//void MainWindow::on_OpenCom_clicked()
//{
//    if(ui->pushButton_4->text()==QString("打开串口"))
//    {
//        //设置串口名
//        serial.setPortName(ui->comboBox->currentText());
//        //设置波特率
//        serial.setBaudRate(ui->comboBox_2->currentText().toInt());
//        //设置数据位数
//        switch(ui->comboBox_3->currentIndex())
//        {
//            case 0: serial.setDataBits(QSerialPort::Data8); break;
//            case 1: serial.setDataBits(QSerialPort::Data7); break;
//            case 2: serial.setDataBits(QSerialPort::Data6); break;
//            case 3: serial.setDataBits(QSerialPort::Data5); break;
//            default: break;
//        }
//        //设置奇偶校验
//        switch(ui->comboBox_4->currentIndex())
//        {
//            case 0: serial.setParity(QSerialPort::NoParity); break;
//            case 1: serial.setParity(QSerialPort::OddParity); break;
//            case 2: serial.setParity(QSerialPort::EvenParity); break;
//            case 3: serial.setParity(QSerialPort::MarkParity); break;
//            case 4: serial.setParity(QSerialPort::SpaceParity); break;
//            default: break;
//        }
//        //设置停止位
//        switch(ui->comboBox_5->currentIndex())
//        {
//            case 0: serial.setStopBits(QSerialPort::OneStop); break;
//            case 1: serial.setStopBits(QSerialPort::OneAndHalfStop); break;
//            case 2: serial.setStopBits(QSerialPort::TwoStop); break;
//            default: break;
//        }
//        //设置流控制
//        serial.setFlowControl(QSerialPort::NoFlowControl);
//        //打开串口
//        if(!serial.open(QIODevice::ReadWrite))
//        {
//            QMessageBox::about(this, "提示", "无法打开串口！");
//            return;
//        }
//        //下拉菜单控件失能
//        ui->comboBox->setEnabled(false);
//        ui->comboBox_2->setEnabled(false);
//        ui->comboBox_3->setEnabled(false);
//        ui->comboBox_4->setEnabled(false);
//        ui->comboBox_5->setEnabled(false);
//        ui->pushButton_4->setText(QString("关闭串口"));
//        //发送按键使能
//        ui->pushButton_2->setEnabled(true);
//        ui->pushButton_5->setEnabled(true);
//    }
//    else
//    {
//        //关闭串口
//        serial.close();
//        //下拉菜单控件使能
//        ui->comboBox->setEnabled(true);
//        ui->comboBox_2->setEnabled(true);
//        ui->comboBox_3->setEnabled(true);
//        ui->comboBox_4->setEnabled(true);
//        ui->comboBox_5->setEnabled(true);
//        ui->pushButton_4->setText(QString("打开串口"));
//        //发送按键失能
//        ui->pushButton_2->setEnabled(false);
//        ui->pushButton_5->setEnabled(false);
//    }

//}

//void MainWindow::on_SearchCom_clicked()
//{
//    ui->comboBox->clear();
//    //通过QSerialPortInfo查找可用串口
//    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//    {
//        ui->comboBox->addItem(info.portName());
//    }
//}

//void MainWindow::on_SendData_clicked()
//{
//    //获取界面上的数据并转换成utf8格式的字节流
//    QByteArray data = ui->textEdit_2->toPlainText().toUtf8();
//    serial.write(data);
//}

//void MainWindow::on_ClearData_clicked()
//{
//    ui->textEdit->clear();
//}

//void MainWindow::on_AutoSend_clicked()
//{
//    if(serial.isOpen())
//    {
//        if(ui->pushButton_5->text()==QString("打开发送数据"))
//        {
//            ui->pushButton_5->setText(QString("关闭发送数据"));
////            d_And_c buf;
//////            buf.d = 10.99;
//////            char a[8]{0};
////            double initD = 10.99;
////            memcpy(&buf,&initD,sizeof(double));
////            serial.write(buf.c);
////            qDebug() << "特使" << buf.d;
//        }
//        else {
//            ui->pushButton_5->setText(QString("打开发送数据"));
//        }
//    }

//}
