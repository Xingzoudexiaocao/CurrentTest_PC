﻿#include "historydetail.h"

HistoryDetail::HistoryDetail(QWidget *parent, USB_HID *hid, ComData *comD) : QGraphicsView(new QGraphicsScene, parent)
{
    qDebug() << "HistoryDetail构造函数。";

    m_ComData = comD;
    m_UsbHid = hid;

    DataSize = 60000;        // 3600000
    d_timeStamps = new double[60000];	// The timestamps for the data series
    d_dataSeriesV = new double[60000];	// The values for the data series A
    d_dataSeriesA = new double[60000];
    d_currentIndex = 1;
//    d_timeStamps = d_time;
//    d_dataSeriesV = d_V;
//    d_dataSeriesA = d_A;

    QString bnt_qss2 = "QPushButton{border-image: url(:/ButtonNormal.png);color:white; border:1px solid black;text-align:left; padding:2px; font-size:22px;\
            border-top-left-radius:8px;         \
            border-top-right-radius:8px;        \
            border-bottom-left-radius:8px;     \
            border-bottom-right-radius:8px }   \
        QPushButton:hover{border-image: url(:/ButtonHover.png);color:#f0f0f0 ;}  \
        QPushButton:pressed{border-image: url(:/ButtonPressed.png);color:#e0e0e0 ;}\
        QPushButton:disabled{ color:#585858 ;}";
    int fontId = QFontDatabase::addApplicationFont(":/ZhuoJianGanLanJianTi.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    font.setFamily(fontFamilies.at(0));

    // Chart Viewer
    m_ChartViewer = new QChartViewer(this);
    m_ChartViewer->setGeometry(4, -16 + (m_ComData->gUiSize->height() - 78 - 10) / 2 - 16, m_ComData->gUiSize->width() - 280 - 5, 505);
    connect(m_ChartViewer, SIGNAL(viewPortChanged()),this, SLOT(onViewPortChanged()));
    connect(m_ChartViewer, SIGNAL(mouseMovePlotArea(QMouseEvent*)), this,
        SLOT(onMouseMovePlotArea(QMouseEvent*)));

    // Horizontal scroll bar
    m_HScrollBar = new QScrollBar(Qt::Horizontal, this);
    m_HScrollBar->setGeometry(4, -16 + (m_ComData->gUiSize->height() - 78 - 10) - 16 - 12, m_ComData->gUiSize->width() - 280 - 5, 17);
    connect(m_HScrollBar, SIGNAL(valueChanged(int)),this, SLOT(onHScrollBarChanged(int)));

    // Chart Viewer 2
    m_ChartViewer_2 = new QChartViewer(this);
    m_ChartViewer_2->setGeometry(4, -16, m_ComData->gUiSize->width() - 280 - 5, 505);
    connect(m_ChartViewer_2, SIGNAL(viewPortChanged()),this, SLOT(onViewPortChanged_2()));
    connect(m_ChartViewer_2, SIGNAL(mouseMovePlotArea(QMouseEvent*)), this,
        SLOT(onMouseMovePlotArea_2(QMouseEvent*)));

    // Horizontal scroll bar
    m_HScrollBar_2 = new QScrollBar(Qt::Horizontal, this);
    m_HScrollBar_2->setGeometry(4, -16 + (m_ComData->gUiSize->height() - 78 - 10) / 2 - 12, m_ComData->gUiSize->width() - 280 - 5, 17);
    connect(m_HScrollBar_2, SIGNAL(valueChanged(int)), this, SLOT(onHScrollBarChanged_2(int)));

//    onHScrollBarChanged(1); // 初始化显示表格
//    onHScrollBarChanged_2(1);
////    onMouseMovePlotArea(nullptr);
////    m_ChartViewer_2->updateDisplay();

    // Initially set the mouse to drag to scroll mode.
    onMouseUsageChanged( Chart::MouseUsageScroll);      // 初始化表格鼠标事件的选中和缩放问题

    // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
    m_ChartViewer->setMouseWheelZoomRatio(1.1);
    m_ChartViewer_2->setMouseWheelZoomRatio(1.1);

//    updateChart();      // 初始化显示表格

    m_EjectSubButton = new QPushButton(this);
    m_EjectSubButton->setGeometry(2, 0, parent->width() - 10, 10);
    m_EjectSubButton->setStyleSheet(bnt_qss2);
    connect(m_EjectSubButton, &QAbstractButton::clicked, this, &HistoryDetail::slotSubButtonClick);

    m_SubFrame = new QFrame(this);
    m_SubFrame->setGeometry(2, 10, parent->width() - 10, 200);
    m_SubFrame->setObjectName("FrameQss");
    m_SubFrame->setStyleSheet("QFrame#FrameQss {border:1px solid black; background-color: rgb(210, 210, 210, 180);\
                            border-top-left-radius:4px;         \
                            border-top-right-radius:4px;        \
                            border-bottom-left-radius:4px;      \
                            border-bottom-right-radius:4px}");
    m_SubFrame->setVisible(false);

    historyFile = new QPushButton(m_SubFrame);
    historyFile->setStyleSheet("QPushButton {font-family:arial; text-align:left; padding:5px; font-size:18px; border:1px solid #000000;}");
    historyFile->setGeometry(150, 5, parent->width() - 200, 30);
//    historyFile->setFrameShape(QFrame::NoFrame);
    historyFile->setToolTip("点击加载文件");
    historyFile->setText("");
//    historyFile->setText("设置线程优先级");
    connect(historyFile, &QAbstractButton::clicked, this, &HistoryDetail::slotHistoryOpen);
    historyOpen = new QPushButton( " 加载文件", m_SubFrame);
    historyOpen->setGeometry(10, 5, 130, 30);
    historyOpen->setStyleSheet(bnt_qss2);
    historyOpen->setFont(font);
    historyOpen->setToolTip("点击加载文件");
    connect(historyOpen, &QAbstractButton::clicked, this, &HistoryDetail::slotHistoryOpen);

    m_DoubleSlider = new DoubleSlider(m_SubFrame);
    m_DoubleSlider->setGeometry(10, 35, parent->width() - 50, 50);
//    m_DoubleSlider->setLabel("标题");
    connect(m_DoubleSlider, SIGNAL(mouseReleaseSignal()), this, SLOT(mouseReleaseSlot()));
//    m_DoubleSlider->setRange(1, 60000);
//    m_DoubleSlider->setMinValue(1);
//    m_DoubleSlider->setMaxValue(60000);
//    m_DoubleSlider->setSingleStep(1);

    QLabel *SliderTitle = new QLabel(m_SubFrame);
    SliderTitle->setGeometry(parent->width() / 2 - 100, 35, 200, 30);
    SliderTitle->setStyleSheet("QLabel { text-align:left; font-size:24px; color:#DC143C;}");
    SliderTitle->setFont(font);
    SliderTitle->setText("时间轴");

    BeginT = new QLabel(m_SubFrame);
    BeginT->setGeometry(10, 35, 200, 30);
    BeginT->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");

    LastT = new QLabel(m_SubFrame);
    LastT->setGeometry(parent->width() - 150, 35, 200, 30);
    LastT->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");


    BeginSelectT = new QLabel(m_SubFrame);
    BeginSelectT->setGeometry(parent->width() / 2 - 100 - 200, 100, 200, 30);
    BeginSelectT->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");
    LastSelectT = new QLabel(m_SubFrame);
    LastSelectT->setGeometry(parent->width() / 2 - 100 + 200, 100, 200, 30);
    LastSelectT->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");
    QLabel *BeginSelectTitle = new QLabel(m_SubFrame);
    BeginSelectTitle->setGeometry(parent->width() / 2 - 100 - 200 - 140, 100 - 2, 140, 30);
    BeginSelectTitle->setStyleSheet("QLabel { text-align:left; font-size:20px; color:#DC143C;}");
    BeginSelectTitle->setFont(font);
    BeginSelectTitle->setText("波形起始时间：");
    QLabel *LastSelectTitle = new QLabel(m_SubFrame);
    LastSelectTitle->setGeometry(parent->width() / 2 - 100 + 200 - 140, 100 - 2, 140, 30);
    LastSelectTitle->setStyleSheet("QLabel { text-align:left; font-size:20px; color:#DC143C;}");
    LastSelectTitle->setFont(font);
    LastSelectTitle->setText("波形结束时间：");

    m_Currnet_Avg = new QLabel(m_SubFrame);
    m_Currnet_Avg->setGeometry(parent->width() / 2 - 300, 130, 200, 30);
    m_Currnet_Avg->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");
    m_Currnet_Max = new QLabel(m_SubFrame);
    m_Currnet_Max->setGeometry(parent->width() / 2, 130, 200, 30);
    m_Currnet_Max->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");
    m_Currnet_Min = new QLabel(m_SubFrame);
    m_Currnet_Min->setGeometry(parent->width() / 2 + 300, 130, 200, 30);
    m_Currnet_Min->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");
    QLabel *m_Cur_Title1 = new QLabel(m_SubFrame);
    m_Cur_Title1->setGeometry(parent->width() / 2 - 300 - 120, 130 - 2, 120, 30);
    m_Cur_Title1->setStyleSheet("QLabel { text-align:left; font-size:20px; color:#DC143C;}");
    m_Cur_Title1->setFont(font);
    m_Cur_Title1->setText("电流平均值：");
    QLabel *m_Cur_Title2 = new QLabel(m_SubFrame);
    m_Cur_Title2->setGeometry(parent->width() / 2 - 120, 130 - 2, 120, 30);
    m_Cur_Title2->setStyleSheet("QLabel { text-align:left; font-size:20px; color:#DC143C;}");
    m_Cur_Title2->setFont(font);
    m_Cur_Title2->setText("电流最大值：");
    QLabel *m_Cur_Title3 = new QLabel(m_SubFrame);
    m_Cur_Title3->setGeometry(parent->width() / 2 + 300 - 120, 130 - 2, 120, 30);
    m_Cur_Title3->setStyleSheet("QLabel { text-align:left; font-size:20px; color:#DC143C;}");
    m_Cur_Title3->setFont(font);
    m_Cur_Title3->setText("电流最小值：");


    m_Voltage_Avg = new QLabel(m_SubFrame);
    m_Voltage_Avg->setGeometry(parent->width() / 2 - 300, 160, 200, 30);
    m_Voltage_Avg->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");
    m_Voltage_Max = new QLabel(m_SubFrame);
    m_Voltage_Max->setGeometry(parent->width() / 2, 160, 200, 30);
    m_Voltage_Max->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");
    m_Voltage_Min = new QLabel(m_SubFrame);
    m_Voltage_Min->setGeometry(parent->width() / 2 + 300, 160, 200, 30);
    m_Voltage_Min->setStyleSheet("QLabel {font-family:Arial; text-align:left; font-size:20px; color:#DC143C;}");
    QLabel *m_Vol_Title1 = new QLabel(m_SubFrame);
    m_Vol_Title1->setGeometry(parent->width() / 2 - 300 - 120, 160 - 2, 120, 30);
    m_Vol_Title1->setStyleSheet("QLabel { text-align:left; font-size:20px; color:#DC143C;}");
    m_Vol_Title1->setFont(font);
    m_Vol_Title1->setText("电压平均值：");
    QLabel *m_Vol_Title2 = new QLabel(m_SubFrame);
    m_Vol_Title2->setGeometry(parent->width() / 2 - 120, 160 - 2, 120, 30);
    m_Vol_Title2->setStyleSheet("QLabel { text-align:left; font-size:20px; color:#DC143C;}");
    m_Vol_Title2->setFont(font);
    m_Vol_Title2->setText("电压最大值：");
    QLabel *m_Vol_Title3 = new QLabel(m_SubFrame);
    m_Vol_Title3->setGeometry(parent->width() / 2 + 300 - 120, 160 - 2, 120, 30);
    m_Vol_Title3->setStyleSheet("QLabel { text-align:left; font-size:20px; color:#DC143C;}");
    m_Vol_Title3->setFont(font);
    m_Vol_Title3->setText("电压最小值：");

    initDisplay();      // 显示初始化
    updateInfomationDisplay();
}

HistoryDetail::~HistoryDetail()
{
    qDebug() << "HistoryDetail析构函数。";
    delete [] d_timeStamps;
    delete [] d_dataSeriesV;
    delete [] d_dataSeriesA;
}

void HistoryDetail::mouseReleaseSlot(void)
{
//    qDebug() << "moureRelease";
    const float EPSINON = 0.1;
    if(abs(DifferValue - m_DoubleSlider->maxValue() + m_DoubleSlider->minValue()) > EPSINON)
    {
//        qDebug() << "更新UI";
        if(m_UsbHid->dev_handle != nullptr)
        {
            qDebug() << "请点击暂停采集，再执行该操作！";
             QMessageBox::critical(this, "提示", "请点击暂停采集，再执行该操作！");
            return;
        }
        DifferValue = m_DoubleSlider->maxValue() - m_DoubleSlider->minValue();
        BeginSelectT->setText(QDateTime::fromMSecsSinceEpoch(BeginDateTime + (qint64)m_DoubleSlider->minValue() - 1).toString("hh:mm:ss.zzz"));
        LastSelectT->setText(QDateTime::fromMSecsSinceEpoch(BeginDateTime + (qint64)m_DoubleSlider->maxValue() - 1).toString("hh:mm:ss.zzz"));
        loadingDataByTime((qint64)m_DoubleSlider->minValue(), (qint64)m_DoubleSlider->maxValue());
        updateInfomationDisplay();
    }
}

void HistoryDetail::initDisplay(void)
{
    DifferValue = 0;
    CountSize = 1;
    SelectSize = 1;
    IntervalValue = 1;
    QDateTime curT = QDateTime::currentDateTime();
    BeginDateTime = curT.toMSecsSinceEpoch();
    BeginT->setText(curT.toString("hh:mm:ss.zzz"));
    LastT->setText(curT.toString("hh:mm:ss.zzz"));
    BeginSelectT->setText(curT.toString("hh:mm:ss.zzz"));
    LastSelectT->setText(curT.toString("hh:mm:ss.zzz"));

    m_DoubleSlider->setRange(1, 1);
    m_DoubleSlider->setMinValue(1);
    m_DoubleSlider->setMaxValue(1);
    m_DoubleSlider->setSingleStep(1);

    memset(&m_Current_DataInfo, 0, sizeof (m_Current_DataInfo));
    memset(&m_Voltage_DataInfo, 0, sizeof (m_Voltage_DataInfo));


    updateChart();      // 初始化显示表格
}

void HistoryDetail::loadingDataByTime(qint64 t1, qint64 t2)
{
    if(t1 > CountSize || t2 > CountSize || t1 > t2)
    {
        qDebug() << "非法数据";
        return;
    }
    SelectSize = t2 - t1 + 1;
    IntervalValue = SelectSize / DataSize + 1;
    qDebug() << "t1 = " << t1 << "t2 = " << t2;
    qDebug() << "IntervalValue = " << IntervalValue;

    // 根据时间间隔抽取数据库中的数据
    QSqlQueryModel sqlModel;
    QString strQuery = "select * from stm32_data where (id % " + QString::number(IntervalValue, 10)
            + " == 0) and id >= " + QString::number(t1, 10) + " and id <= " + QString::number(t2, 10) + " ";
//    QString  strQuery = "select * from stm32_data where (id <= 60000)";
    sqlModel.setQuery(strQuery);
    while(sqlModel.canFetchMore())
    {
        sqlModel.fetchMore();
    }
    qDebug()<< "Update Chart Data:" <<sqlModel.rowCount() << QDateTime::currentDateTime();
    d_currentIndex = sqlModel.rowCount();
    if(d_currentIndex > DataSize)
        d_currentIndex = DataSize;
    for(int i = 0; i < d_currentIndex; i++)
    {
        QDateTime now = QDateTime::fromMSecsSinceEpoch(sqlModel.record(i).value("time").toLongLong());
        *(d_timeStamps + i) = Chart::chartTime2(now.toTime_t())
                             + ((double)now.time().msec() / 10) * 0.01;     //     / 10 * 0.01
        *(d_dataSeriesV + i) = sqlModel.record(i).value("voltage").toDouble();
        *(d_dataSeriesA + i) = sqlModel.record(i).value("current").toDouble();
    }
    qDebug()<< "Update Chart Data Success: "<< QDateTime::currentDateTime();

    // 计算选中数据的平均值
    strQuery = "select avg(current), max(current), min(current), avg(voltage), max(voltage), min(voltage) from stm32_data where id >= "
            + QString::number(t1, 10) + " and id <= " + QString::number(t2, 10) + " ";
    sqlModel.setQuery(strQuery);
//    qDebug() << "个数 = " << sqlModel.record().count();
//    double curBuf = sqlModel.record(0).value(0).toDouble();
//    qDebug() << "电流平均值数 = " << QString::number(curBuf, 'f', 10) << QDateTime::currentDateTime();
//    double volBuf = sqlModel.record(0).value(3).toDouble();
//    qDebug() << "电压平均值数 = " << QString::number(volBuf, 'f', 10) << QDateTime::currentDateTime();
    m_Current_DataInfo.average = sqlModel.record(0).value(0).toDouble();
    m_Current_DataInfo.maximun = sqlModel.record(0).value(1).toDouble();
    m_Current_DataInfo.minimun = sqlModel.record(0).value(2).toDouble();
    m_Voltage_DataInfo.average = sqlModel.record(0).value(3).toDouble();
    m_Voltage_DataInfo.maximun = sqlModel.record(0).value(4).toDouble();
    m_Voltage_DataInfo.minimun = sqlModel.record(0).value(5).toDouble();



//    m_HScrollBar->setValue(1000000000);
//    m_HScrollBar_2->setValue(1000000000);
//    onHScrollBarChanged(1000000000); // 初始化显示表格
//    onHScrollBarChanged_2(1000000000);
//    onViewPortChanged();
//    onViewPortChanged_2();
//    m_ChartViewer->setViewPortLeft(1);
//    m_ChartViewer->updateViewPort(true, false);
//    m_ChartViewer_2->setViewPortLeft(1);
//    m_ChartViewer_2->updateViewPort(true, false);

    updateChart();
}

void HistoryDetail::updateInfomationDisplay(void)
{
    if(m_Current_DataInfo.average < 0.001)
        m_Currnet_Avg->setText(QString::number(m_Current_DataInfo.average * 1000, 'f', 3) + "uA");
    else
        m_Currnet_Avg->setText(QString::number(m_Current_DataInfo.average, 'f', 3) + "mA");
    if(m_Current_DataInfo.maximun < 0.001)
        m_Currnet_Max->setText(QString::number(m_Current_DataInfo.maximun * 1000, 'f', 3) + "uA");
    else
        m_Currnet_Max->setText(QString::number(m_Current_DataInfo.maximun, 'f', 3) + "mA");
    if(m_Current_DataInfo.minimun < 0.001)
        m_Currnet_Min->setText(QString::number(m_Current_DataInfo.minimun * 1000, 'f', 3) + "uA");
    else
        m_Currnet_Min->setText(QString::number(m_Current_DataInfo.minimun, 'f', 3) + "mA");

    m_Voltage_Avg->setText(QString::number(m_Voltage_DataInfo.average, 'f', 3) + "V");
    m_Voltage_Max->setText(QString::number(m_Voltage_DataInfo.maximun, 'f', 3) + "V");
    m_Voltage_Min->setText(QString::number(m_Voltage_DataInfo.minimun, 'f', 3) + "V");
}

void HistoryDetail::slotSubButtonClick(void)
{
    if(m_SubFrame->isVisible())
        m_SubFrame->setVisible(false);
    else
        m_SubFrame->setVisible(true);
}

void HistoryDetail::slotHistoryOpen(void)
{
    if(m_UsbHid->dev_handle != nullptr)
    {
        qDebug() << "USB正在运行，无法加载文件！";
         QMessageBox::critical(this, "提示", "USB正在运行，无法加载文件！");
        return;
    }
    qDebug() << qApp->applicationDirPath();
    QString fileName=QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("历史数据"),qApp->applicationDirPath() + "/iSCAN_Data",
                                                  QString::fromLocal8Bit("bin File(*.db)"));//新建文件打开窗口
    if (fileName.isEmpty())//如果未选择文件便确认，即返回
        return;
//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly))
//        qDebug() << file.errorString();//文件打开错误显示错误信息
//    QByteArray arry=file.readAll();//读取文件
//    file.close();
    qDebug() << "打开文件：" << fileName;
    historyFile->setText(fileName);
//    int length=arry.size();//计算长度
//    qDebug() << length;
//    historyView->LoadingData(fileName);     // 更新表格数据
    LoadingData(fileName);
}

void HistoryDetail::ReceiveTest(void)
{
    qDebug() << "接收到信号！";
}

void HistoryDetail::LoadingData(QString fileName)
{
   qDebug() << "update view";
//   QString ConnectName;
//   {
       QSqlDatabase myDb;
       if(QSqlDatabase::contains("HistoryData"))
         myDb = QSqlDatabase::addDatabase("QSQLITE", "HistoryData");
       else
         myDb = QSqlDatabase::addDatabase("QSQLITE");
       myDb.setDatabaseName(fileName);    // QApplication::applicationDirPath() + "CONFIG.db"     不能包含字符
    //     db.setUserName("admin");
    //     db.setPassword("admin");
       if (!myDb.open())     // if (!db.open("admin","admin"))
       {
           qDebug() << "打开数据库文件失败！";
           QMessageBox::critical(this, "提示", "打开数据库文件失败！");
           return;
       }
       QSqlQueryModel sqlModel;
       QString   strQuery = "select count(*) from stm32_data";        // select count(*) from table
       sqlModel.setQuery(strQuery);
       CountSize = sqlModel.record(0).value(0).toLongLong();
//       ConnectName = QSqlDatabase::database().connectionName();
//       myDb.close();
//   }
//   QSqlDatabase::removeDatabase(ConnectName);

   if(CountSize < 10000)
   {
       CountSize = 1;
       qDebug() << "数据文件数据量太少！";
       QMessageBox::critical(this, "提示", "无法打开当前文件，文件数据量太少！");
       return;
   }
   qDebug() << "数据个数 = " << CountSize << QDateTime::currentDateTime();

   strQuery = "select time from stm32_data where id= (select min(id) from stm32_data)";        // select count(*) from table
   sqlModel.setQuery(strQuery);
   BeginDateTime = sqlModel.record(0).value(0).toLongLong();
   qDebug() << "起始时间 = " << BeginDateTime << QDateTime::fromMSecsSinceEpoch(BeginDateTime) << QDateTime::currentDateTime();

   // 更新UI
   BeginT->setText(QDateTime::fromMSecsSinceEpoch(BeginDateTime).toString("hh:mm:ss.zzz"));
   LastT->setText(QDateTime::fromMSecsSinceEpoch(BeginDateTime + CountSize - 1).toString("hh:mm:ss.zzz"));
   m_DoubleSlider->setRange(1, CountSize);
   m_DoubleSlider->setMinValue(1);
   m_DoubleSlider->setMaxValue(CountSize);
   m_DoubleSlider->setSingleStep(1);
   BeginSelectT->setText(QDateTime::fromMSecsSinceEpoch(BeginDateTime).toString("hh:mm:ss.zzz"));
   LastSelectT->setText(QDateTime::fromMSecsSinceEpoch(BeginDateTime + CountSize - 1).toString("hh:mm:ss.zzz"));

   loadingDataByTime((qint64)m_DoubleSlider->minValue(), (qint64)m_DoubleSlider->maxValue());
   updateInfomationDisplay();
}

void HistoryDetail::ClearData(void)
{

}

void HistoryDetail::UpdateChartData(void)
{

}

void HistoryDetail::updateChart()
{
//    m_ChartViewer->updateViewPort(true, false);     // 这里可能会有问题
//    m_ChartViewer_2->updateViewPort(true, false);
    onChartUpdateTimer(m_ChartViewer);
    onChartUpdateTimer(m_ChartViewer_2);
}

void HistoryDetail::onMouseMovePlotArea(QMouseEvent *)
{
    trackLineLabel((XYChart *)m_ChartViewer->getChart(), m_ChartViewer->getPlotAreaMouseX(), 0);
    m_ChartViewer->updateDisplay();
}
void HistoryDetail::onMouseMovePlotArea_2(QMouseEvent *)
{
    trackLineLabel((XYChart *)m_ChartViewer_2->getChart(), m_ChartViewer_2->getPlotAreaMouseX(), 1);
    m_ChartViewer_2->updateDisplay();
}

void HistoryDetail::onMouseUsageChanged(int mouseUsage)
{
    m_ChartViewer->setMouseUsage(mouseUsage);
    m_ChartViewer_2->setMouseUsage(mouseUsage);
}

void HistoryDetail::onHScrollBarChanged(int value)
{
    if (!m_ChartViewer->isInViewPortChangedEvent())
    {
        // Set the view port based on the scroll bar
        int scrollBarLen = m_HScrollBar->maximum() + m_HScrollBar->pageStep();
        m_ChartViewer->setViewPortLeft(value / (double)scrollBarLen);

        // Update the chart display without updating the image maps. (We can delay updating
        // the image map until scrolling is completed and the chart display is stable.)
        m_ChartViewer->updateViewPort(true, false);
    }
}
void HistoryDetail::onHScrollBarChanged_2(int value)
{
    if (!m_ChartViewer_2->isInViewPortChangedEvent())
    {
        // Set the view port based on the scroll bar
        int scrollBarLen = m_HScrollBar_2->maximum() + m_HScrollBar_2->pageStep();
        m_ChartViewer_2->setViewPortLeft(value / (double)scrollBarLen);
//        qDebug() << "scrollBarLen = " << scrollBarLen;
//        qDebug() << "value = " << value;
        // Update the chart display without updating the image maps. (We can delay updating
        // the image map until scrolling is completed and the chart display is stable.)
        m_ChartViewer_2->updateViewPort(true, false);
    }
}

void HistoryDetail::onViewPortChanged()
{
    // In addition to updating the chart, we may also need to update other controls that
    // changes based on the view port.
    updateControls(m_ChartViewer, m_HScrollBar);

    // Update the chart if necessary
    if (m_ChartViewer->needUpdateChart())
        drawChart(m_ChartViewer, 0);
}
void HistoryDetail::onViewPortChanged_2()
{
    // In addition to updating the chart, we may also need to update other controls that
    // changes based on the view port.
    updateControls(m_ChartViewer_2, m_HScrollBar_2);

    // Update the chart if necessary
    if (m_ChartViewer_2->needUpdateChart())
        drawChart(m_ChartViewer_2, 1);
}

void HistoryDetail::updateControls(QChartViewer *viewer, QScrollBar *bar)
{
    // The logical length of the scrollbar. It can be any large value. The actual value does
    // not matter.
    const int scrollBarLen = 1000000000;

    // Update the horizontal scroll bar
    bar->setEnabled(viewer->getViewPortWidth() < 1);
    bar->setPageStep((int)ceil(viewer->getViewPortWidth() * scrollBarLen));
    bar->setSingleStep(min(scrollBarLen / 100, bar->pageStep()));
    bar->setRange(0, scrollBarLen - bar->pageStep());
    bar->setValue((int)(0.5 + viewer->getViewPortLeft() * scrollBarLen));
}

//
// Draw chart
//
void HistoryDetail::drawChart(QChartViewer *viewer, int index)
{
    // Get the start date and end date that are visible on the chart.
    double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
    double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() +
        viewer->getViewPortWidth());

    // Extract the part of the data arrays that are visible.
    DoubleArray viewPortTimeStamps;
    DoubleArray viewPortDataSeriesA;
    DoubleArray viewPortDataSeriesB;
    DoubleArray viewPortDataSeriesC;

    if (d_currentIndex >= 0)
    {
        // Get the array indexes that corresponds to the visible start and end dates
        int startIndex = (int)floor(Chart::bSearch(DoubleArray(d_timeStamps, d_currentIndex), viewPortStartDate));
        int endIndex = (int)ceil(Chart::bSearch(DoubleArray(d_timeStamps, d_currentIndex), viewPortEndDate));
        int noOfPoints = endIndex - startIndex + 1;

        // Extract the visible data
        viewPortTimeStamps = DoubleArray(d_timeStamps+ startIndex, noOfPoints);
        viewPortDataSeriesB = DoubleArray(d_dataSeriesV + startIndex, noOfPoints);
        viewPortDataSeriesC = DoubleArray(d_dataSeriesA + startIndex, noOfPoints);
    }


    //
    // At this stage, we have extracted the visible data. We can use those data to plot the chart.
    //

    //================================================================================
    // Configure overall chart appearance.
    //================================================================================

    // Create an XYChart object of size 640 x 350 pixels
    XYChart *c = new XYChart(m_ComData->gUiSize->width() - 280, (m_ComData->gUiSize->height() - 78 - 10) / 2);     // 1345, 425        m_HScrollBar->width(), 300

    // Set the plotarea at (55, 50) with width 80 pixels less than chart width, and height 80 pixels
    // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
    // as background. Set border to transparent and grid lines to white (ffffff).
    c->setPlotArea(85, 62, c->getWidth() - 85 - 30, c->getHeight() - 100, c->linearGradientColor(0, 50, 0,
        c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

    // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
    c->setClipping();

    // Add a title to the chart using 18pt Arial font
//    if(index == 0)
//        c->addTitle("Voltage with Zoom/Scroll and Track Line", "arial.ttf", 18);
//    else if (index == 1)
//        c->addTitle("Current with Zoom/Scroll and Track Line", "arial.ttf", 18);

    // Add a legend box at (55, 25) using horizontal layout. Use 10pt Arial Bold as font. Set the
    // background and border color to transparent and use line style legend key.
    LegendBox *b = c->addLegend(85, 25, false, "arialbd.ttf", 10);
    b->setBackground(Chart::Transparent);
    b->setLineStyleKey();

    // Set the x and y axis stems to transparent and the label font to 10pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 10);
    c->yAxis()->setLabelStyle("arial.ttf", 10);

    // Set the y-axis tick length to 0 to disable the tick and put the labels closer to the axis.
    c->yAxis()->setTickLength(0);

    // Add axis title using 12pt Arial Bold Italic font
    if(index == 0)
        c->yAxis()->setTitle("Voltage ( V )", "arialbd.ttf", 12);
    else if (index == 1)
        c->yAxis()->setTitle("Current ( mA )", "arialbd.ttf", 12);


    //================================================================================
    // Add data to chart
    //================================================================================

    //
    // In this example, we represent the data by lines. You may modify the code below to use other
    // representations (areas, scatter plot, etc).
    //


    // Add a line layer for the lines, using a line width of 2 pixels
    LineLayer *layer = c->addLineLayer();
    layer->setLineWidth(2);
    layer->setFastLineMode();

    // Now we add the 3 data series to a line layer, using the color red (ff0000), green (00cc00)
    // and blue (0000ff)
    layer->setXData(viewPortTimeStamps);
    char buffer[1024];
    if(index == 0) {
        if(d_currentIndex >= 0) {
            sprintf(buffer, " <*bgColor=ffffff*> <*color=00cc00*> <*size=14px*>");   //  %.3f V  , d_dataSeriesV[DataSize - 1]
        }
        layer->addDataSet(viewPortDataSeriesB, 0x00cc00, buffer);   // , buffer
        c->yAxis()->setMinTickInc(0.1);
        c->yAxis()->setDateScale(0, 7.5);           // 固定坐标轴0-7.5V
    }
    else if(index == 1) {
        if(d_currentIndex >= 0) {
//            double d = d_dataSeriesA[DataSize - 1];
//            if(d < 1) {
//                sprintf(buffer, " <*bgColor=ffffff*> <*color=0000ff*> <*size=14px*> %.3f uA", d * 1000);
//            } else {
//                sprintf(buffer, " <*bgColor=ffffff*> <*color=0000ff*> <*size=14px*> %.2f mA", d);
//            }
            sprintf(buffer, " <*bgColor=ffffff*> <*color=0000ff*> <*size=14px*>");
        }
        layer->addDataSet(viewPortDataSeriesC, 0x0000ff, buffer);       // , buffer
//        c->yAxis()->setMinTickInc(0.1);
//        if(fixCurrentValue > 0)
//            c->yAxis()->setDateScale(0, fixCurrentValue);
    }


    //================================================================================
    // Configure axis scale and labelling
    //================================================================================

    // Set the x-axis as a date/time axis with the scale according to the view port x range.
    if (d_currentIndex >= 0)
        c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

    // For the automatic axis labels, set the minimum spacing to 75/30 pixels for the x/y axis.
    c->xAxis()->setTickDensity(75);
    c->yAxis()->setTickDensity(30);
//    c->yAxis()->setLinearScale(0, 1000, 100, 10);
//    c->yAxis()->setTickLength(8, 4);
//    c->yAxis()->setLength(1);
//    c->yAxis()->setAutoScale();

    //
    // In this demo, the time range can be from many hours to a few seconds. We can need to define
    // the date/time format the various cases.
    //

    c->yAxis()->setLabelFormat("{value|3}");
    if(index == 1)
    {
        double yMax = *std::max_element(viewPortDataSeriesC.data, viewPortDataSeriesC.data + viewPortDataSeriesC.len);
        if(yMax <= 0)
            yMax = 120;
//    qDebug() << "yMax = " << QString::number(yMax, 'f', 10);
        if(yMax < 0.001) {
            c->yAxis()->setLabelFormat("{={value}*1000|3}");
            c->yAxis()->setTitle("Current ( uA )", "arialbd.ttf", 12);
        } else if(yMax < 1000) {
            c->yAxis()->setLabelFormat("{value|3}");
            c->yAxis()->setTitle("Current ( mA )", "arialbd.ttf", 12);
        } else {
            c->yAxis()->setLabelFormat("{value|2}");
            c->yAxis()->setTitle("Current ( mA )", "arialbd.ttf", 12);
        }
    }

    // If all ticks are hour algined, we use "hh:nn<*br*>mmm dd" in bold font as the first label of
    // the Day, and "hh:nn" for other labels.
    c->xAxis()->setFormatCondition("align", 3600);
    c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(), "<*font=bold*>{value|hh:nn<*br*>mmm dd}",
        Chart::AllPassFilter(), "{value|hh:nn}");

    // If all ticks are minute algined, then we use "hh:nn" as the label format.
    c->xAxis()->setFormatCondition("align", 60);
    c->xAxis()->setLabelFormat("{value|hh:nn}");

    // If all other cases, we use "hh:nn:ss" as the label format.
    c->xAxis()->setFormatCondition("else");
    c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

    // We make sure the tick increment must be at least 1 second.
    c->xAxis()->setMinTickInc(IntervalValue / 1000);       // 0.001

    //================================================================================
    // Output the chart
    //================================================================================

    // We need to update the track line too. If the mouse is moving on the chart (eg. if
    // the user drags the mouse on the chart to scroll it), the track line will be updated
    // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
    if (!viewer->isInMouseMoveEvent())
    {
        trackLineLabel(c, (0 == viewer->getChart()) ? c->getPlotArea()->getRightX() :
            viewer->getPlotAreaMouseX(), index);
    }

    // Set the chart image to the QChartViewer
    delete viewer->getChart();
    viewer->setChart(c);
}


void HistoryDetail::trackLineLabel(XYChart *c, int mouseX, int index)
{
    // Clear the current dynamic layer and get the DrawArea object to draw on it.
    DrawArea *d = c->initDynamicLayer();

    // The plot area object
    PlotArea *plotArea = c->getPlotArea();

    // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
    double xValue = c->getNearestXValue(mouseX);
    int xCoor = c->getXCoor(xValue);
    if (xCoor < plotArea->getLeftX())
        return;

    // Draw a vertical track line at the x-position
    d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0x888888);

    // Draw a label on the x-axis to show the track line position.
    std::ostringstream xlabel;
    xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss.fff")
        << " <*/font*>";
    TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 10);

    // Restrict the x-pixel position of the label to make sure it stays inside the chart image.
    int xLabelPos = max(0, min(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
    t->draw(xLabelPos, plotArea->getBottomY() + 6, 0xffffff);
    t->destroy();

    // Iterate through all layers to draw the data labels
    for (int i = 0; i < c->getLayerCount(); ++i) {
        Layer *layer = c->getLayerByZ(i);

        // The data array index of the x-value
        int xIndex = layer->getXIndexOf(xValue);

        // Iterate through all the data sets in the layer
        for (int j = 0; j < layer->getDataSetCount(); ++j)
        {
            DataSet *dataSet = layer->getDataSetByZ(j);
            const char *dataSetName = dataSet->getDataName();

            // Get the color, name and position of the data label
            int color = dataSet->getDataColor();
            int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

            // Draw a track dot with a label next to it for visible data points in the plot area
            if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) && (color !=
                Chart::Transparent) && dataSetName && *dataSetName)
            {
                d->circle(xCoor, yCoor, 4, 4, color, color);

                std::ostringstream label;
                if(index == 0) {
                    label << "<*font,bgColor=" << hex << color << "*> "
                        << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << "V" << " <*font*>";
                } else if(index == 1) {
                    double bufD = dataSet->getValue(xIndex);
                    if(bufD < 1) {
                        label << "<*font,bgColor=" << hex << color << "*> "
                            << c->formatValue(bufD * 1000, "{value|P4}") << "uA" << " <*font*>";
                    } else {
                        label << "<*font,bgColor=" << hex << color << "*> "
                            << c->formatValue(bufD, "{value|P4}") << "mA" << " <*font*>";
                    }
                }


                t = d->text(label.str().c_str(), "arialbd.ttf", 10);

                // Draw the label on the right side of the dot if the mouse is on the left side the
                // chart, and vice versa. This ensures the label will not go outside the chart image.
                if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
                    t->draw(xCoor + 6, yCoor, 0xffffff, Chart::Left);
                else
                    t->draw(xCoor - 6, yCoor, 0xffffff, Chart::Right);

                t->destroy();
            }
        }
    }
}

void HistoryDetail::onChartUpdateTimer(QChartViewer *viewer)
{
    qint64 initialFullRange = 60;
    qint64 zoomInLimit = 1;

    initialFullRange = initialFullRange * IntervalValue;
    zoomInLimit = zoomInLimit * IntervalValue;

    if (d_currentIndex > 0)
    {
        //
        // As we added more data, we may need to update the full range of the viewport.
        //

        double startDate = 0;
        double endDate = 0;

        if (d_currentIndex > 1)
        {
            startDate = d_timeStamps[0];
            endDate = d_timeStamps[m_ComData->d_currentIndex - 1];
        }
        // Use the initialFullRange (which is 60 seconds in this demo) if this is sufficient.
        double duration = endDate - startDate;
        if (duration < initialFullRange)
            endDate = startDate + initialFullRange;

        // Update the full range to reflect the actual duration of the data. In this case,
        // if the view port is viewing the latest data, we will scroll the view port as new
        // data are added. If the view port is viewing historical data, we would keep the
        // axis scale unchanged to keep the chart stable.
        int updateType = Chart::ScrollWithMax;
        if (viewer->getViewPortLeft() + viewer->getViewPortWidth() < 0.999)
            updateType = Chart::KeepVisibleRange;
        bool scaleHasChanged = viewer->updateFullRangeH("x", startDate, endDate, updateType);

        // Set the zoom in limit as a ratio to the full range
        viewer->setZoomInWidthLimit(zoomInLimit / (viewer->getValueAtViewPort("x", 1) -
            viewer->getValueAtViewPort("x", 0)));

        // Trigger the viewPortChanged event to update the display if the axis scale has changed
        // or if new data are added to the existing axis scale.
        if (scaleHasChanged || (duration < initialFullRange))
            viewer->updateViewPort(true, false);
    }
}
