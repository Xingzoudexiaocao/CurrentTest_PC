#include "historyview.h"

HistoryView::HistoryView(QWidget *parent) : QGraphicsView(new QGraphicsScene, parent)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //![1]
    seriesVoltage = new QLineSeries();
    //![1]

    //![2]
//    seriesVoltage->append(QDateTime::currentDateTime().toMSecsSinceEpoch(), 100);
//    for (int i = 0; i < 500; i++) {
//        QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
//        p.ry() += QRandomGenerator::global()->bounded(20);
//        *seriesVoltage << p;
//    }
    seriesVoltage->setColor("#00cc00");
    //![2]

    //![3]
    chatVoltage = new QChart();
    chatVoltage->addSeries(seriesVoltage);
    chatVoltage->legend()->hide();
    chatVoltage->setGeometry(0, 50, 810, 400);
//    chatVoltage->setTitleFont(font);
//    chatVoltage->setTitle("电压波形图");
    //![3]

    //![4]
    axisVoltageX = new QDateTimeAxis;
    axisVoltageX->setTickCount(10);
    axisVoltageX->setFormat("hh:mm:ss");
//    axisVoltageX->setTitleText("Date");
    chatVoltage->addAxis(axisVoltageX, Qt::AlignBottom);
    seriesVoltage->attachAxis(axisVoltageX);

    axisVoltageY = new QValueAxis;
    axisVoltageY->setTickCount(11);
    axisVoltageY->setLabelFormat("%.3f");
//    axisVoltageY->setLabelsAngle(270);
//    axisVoltageY->setLabelFormat("%i");
//    axisVoltageY->setTitleText("Sunspots count");
    chatVoltage->addAxis(axisVoltageY, Qt::AlignLeft);
    seriesVoltage->attachAxis(axisVoltageY);
    //![4]

//    //![5]
//    chartViewVoltage = new QChartView(chatVoltage, parent);
//    chartViewVoltage->setGeometry(4, 50, 800, 400);
//    chartViewVoltage->setRenderHint(QPainter::Antialiasing);
//    //![5]

    //![1]
    seriesCurrent = new QLineSeries();
    //![1]

    //![2]
//    seriesCurrent->append(QDateTime::currentDateTime().toMSecsSinceEpoch(), 100);
//    for (int i = 0; i < 500; i++) {
//        QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
//        p.ry() += QRandomGenerator::global()->bounded(20);
//        *seriesCurrent << p;
//    }
//    for(int i = 0; i < 1000; i++)
//    {
//        seriesCurrent->append(QDateTime::currentDateTime().toMSecsSinceEpoch() + i, i*2);
//    }
    seriesCurrent->setColor("#0000CC");
    //![2]

    //![3]
    chatCurrent = new QChart();
    chatCurrent->addSeries(seriesCurrent);
    chatCurrent->legend()->hide();
    chatCurrent->setGeometry(0, 425, 810, 400);
//    chatCurrent->setTitleFont(font);
//    chatCurrent->setTitle("电流波形图");
    //![3]

    //![4]
    axisCurrentX = new QDateTimeAxis;
    axisCurrentX->setTickCount(10);
    axisCurrentX->setFormat("hh:mm:ss");
//    axisVoltageX->setTitleText("Date");
    chatCurrent->addAxis(axisCurrentX, Qt::AlignBottom);
    seriesCurrent->attachAxis(axisCurrentX);

    axisCurrentY = new QValueAxis;
    axisCurrentY->setTickCount(11);
    axisCurrentY->setLabelFormat("%.3f");
//    axisCurrentY->setLabelsAngle(270);
//    axisVoltageY->setTitleText("Sunspots count");
    chatCurrent->addAxis(axisCurrentY, Qt::AlignLeft);
    seriesCurrent->attachAxis(axisCurrentY);
    //![4]

//    //![5]
//    chartViewCurrent = new QChartView(chatCurrent, parent);
//    chartViewCurrent->setGeometry(4, 425, 800, 400);
//    chartViewCurrent->setRenderHint(QPainter::Antialiasing);
//    //![5]

    seriesCurrent->connect(seriesCurrent,&QLineSeries::clicked,[](const QPointF& point){
      qDebug() << "point clicked:" << point;
    });
    seriesCurrent->connect(seriesCurrent,&QLineSeries::doubleClicked,[](const QPointF& point){
      qDebug() << "point double clicked:" << point;
    });
    seriesCurrent->connect(seriesCurrent,&QLineSeries::hovered,[](const QPointF& point,bool state){
      qDebug() << "point hovered:" << point << " state:" << (state?"光标移动到图表上":"光标从图表上移开");
    });
    seriesCurrent->connect(seriesCurrent,&QLineSeries::pressed,[](const QPointF& point){
      qDebug() << "point pressed:" << point;

    });
    seriesCurrent->connect(seriesCurrent,&QLineSeries::released,[](const QPointF& point){
      qDebug() << "point released:" << point;
    });
//    chartViewCurrent->connect(seriesCurrent,&QChartView,[](const QPointF& point){
//        qDebug() << "QChartView 事件" << point;
//      });
    seriesMarkV = new QLineSeries();
    seriesMarkA = new QLineSeries();
    seriesMarkV->setColor("#CC0000");
    seriesMarkA->setColor("#CC0000");
    chatVoltage->addSeries(seriesMarkV);
    chatCurrent->addSeries(seriesMarkA);
//    seriesMarkA->attachAxis(axisCurrentX);
//    seriesMarkA->attachAxis(axisCurrentY);

    this->setMouseTracking(true);
//    chatVoltage->setAcceptHoverEvents(true);
//    chatVoltage->setAcceptTouchEvents(true);
//    chatCurrent->setAcceptHoverEvents(true);
//    chatCurrent->setAcceptTouchEvents(true);
    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(chatVoltage);
    scene()->addItem(chatCurrent);

    QString bnt_qss1 = "QPushButton {font-family:arial; font-size:14px; text-align:left; padding:3px 1px 3px 5px;}\
            QPushButton:Enabled {background-color: #0066CC; color:white;}\
            QPushButton:Disabled {background-color: #606060; color:#CCCCCC;}";
    zoomX2 = new QPushButton(QIcon(":/zoomin.png"), "", this);
//    zoomX2->setStyleSheet(bnt_qss1);
    zoomX2->setGeometry(400 + 20 + 300, 60, 20, 20);
//    zoomX2->setText("X2");
    connect(zoomX2,SIGNAL(clicked ()),this,SLOT(ClickZoomX2()));

    zoomD2 = new QPushButton(QIcon(":/zoomout.png"), "", this);
//    zoomD2->setStyleSheet(bnt_qss1);
    zoomD2->setGeometry(400, 60, 20, 20);
//    zoomD2->setText("D2");
    connect(zoomD2,SIGNAL(clicked ()),this,SLOT(ClickZoomD2()));

    zoomX2->setEnabled(false);  zoomD2->setEnabled(false);

    zoomSlide = new QSlider(this);
    zoomSlide->setGeometry(400 + 20, 60, 300, 20);
    zoomSlide->setOrientation(Qt::Horizontal);  // 水平方向
    zoomSlide->setMinimum(1);  // 最小值
    zoomSlide->setMaximum(100);  // 最大值
    zoomSlide->setSingleStep(1); // 步长
    zoomSlide->setPageStep(10);      // 页步长
    zoomSlide->setTickInterval(10);  // 设置刻度间隔
    zoomSlide->setTickPosition(QSlider::TicksAbove);
    zoomSlide->setTracking(false);      // 如果启用跟踪(默认)，滑块在拖动滑块时发出valueChanged()信号。如果禁用跟踪，则滑块仅在用户释放滑块时才发出valueChanged()信号。
    zoomSlide->setEnabled(false);
    connect(zoomSlide, SIGNAL(valueChanged(int)), this, SLOT(ZoomSlideValueChanged(int)));

    unitVol = new QLabel(this);
    unitVol->setStyleSheet("QLabel { text-align:center; font-size:20px; color:#00cc00; background-color:white;}");  //  padding:10px;
    unitVol->setGeometry(20, 60, 100, 20);

    unitCur = new QLabel(this);
    unitCur->setStyleSheet("QLabel { text-align:center; font-size:20px; color:blue; background-color:white;}");  //  padding:10px;
    unitCur->setGeometry(20, 430, 100, 20);

    markLabel = new QLabel(this);
    markLabel->setStyleSheet("QLabel { text-align:center; font-size:16px; color:#CC0000; background-color:white;}");  //  padding:10px;
    markLabel->setGeometry(400, 430, 600, 20);

//    testLCD = new QLCDNumber(this);
//    // 设置能显示的位数
////    testLCD->setDigitCount(25);
//    // 设置显示的模式为十进制
////    testLCD->setMode(QLCDNumber::Dec);
//    // 设置显示外观
////    testLCD->setSegmentStyle(QLCDNumber::Flat);
//    // 设置样式
////    testLCD->setStyleSheet("border: 1px solid green; color: green; background: silver;");
//    testLCD->setGeometry(650, 160, 100, 50);
//    testLCD->display("110mA");
}

void HistoryView::mousePressEvent(QMouseEvent *event)
{
//    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
//    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    qlonglong m_x_A;

    qDebug() << "click chart view" << event->pos();
//    chatVoltage->removeSeries(seriesMarkV);

    if(seriesVoltage->points().count() <= 0 || seriesCurrent->points().count() <= 0)    //
        return;

//    qDebug() << "pos[0] = " << seriesCurrent->points()[0].x();
//    qDebug() << "pos[count-1] = " << seriesCurrent->points()[seriesCurrent->points().count()-1].x();

//    m_x_V = chatVoltage->mapToValue(event->pos()).x();
    if(event->pos().y() < 425)
        m_x_A = (qlonglong)chatVoltage->mapToValue(event->pos()).x();
    else
        m_x_A = (qlonglong)chatCurrent->mapToValue(event->pos()).x();
//    QDateTime pos_x_now = QDateTime::fromMSecsSinceEpoch(m_x_A);
//    QDateTime pos_x_min = QDateTime::fromMSecsSinceEpoch(seriesCurrent->points()[0].x());
//    QDateTime pos_x_max = QDateTime::fromMSecsSinceEpoch(seriesCurrent->points()[seriesCurrent->points().count()-1].x());
//    qDebug() << "pos_x_now = " << pos_x_now;
//    qDebug() << "pos_x_min = " << pos_x_min;
//    qDebug() << "pos_x_max = " << pos_x_max;
//    if(pos_x_now < pos_x_min)     // 测试OK
//        m_x_A = seriesCurrent->points()[0].x();
//    qint64 dec1 = pos_x_now.toMSecsSinceEpoch() - pos_x_min.toMSecsSinceEpoch();
//    qint64 dec2 = pos_x_now.toMSecsSinceEpoch() - pos_x_max.toMSecsSinceEpoch();
//    qDebug() << "   " << dec1 << "    " << dec2;
//    if(dec1 < 0 || dec2 > 0)  // 一旦比较时间，读到的pos_x_now就出错
//        return;
    if(m_x_A < seriesCurrent->points()[0].x())
        m_x_A = seriesCurrent->points()[0].x();
    if(m_x_A > seriesCurrent->points()[seriesCurrent->points().count()-1].x())
        m_x_A = seriesCurrent->points()[seriesCurrent->points().count()-1].x();
    qDebug() << "m_x_A = " << m_x_A << QDateTime::fromMSecsSinceEpoch(m_x_A);

    QSqlQueryModel sqlModel;        // 获取所选中时间对于的id值
    QString   strQuery = "select id, voltage, current from stm32_data where time == " + QString::number(m_x_A, 10);        // select count(*) from table
    sqlModel.setQuery(strQuery);
    qlonglong idRead = sqlModel.record(0).value("id").toLongLong();
    double vol = sqlModel.record(0).value("voltage").toDouble();
    double cur = sqlModel.record(0).value("current").toDouble();
    if(idRead == 0)
    {
        qDebug() << "读取ID失败!" ;
        return;
    }
    zoomIndex = idRead;
    qDebug() << "zoomIndex = " << zoomIndex;

     HistoryView::UpdateMarkLine(m_x_A, vol, cur);

    QGraphicsView::mouseMoveEvent(event);
}

void HistoryView::LoadingData(QString fileName)
{
     qDebug() << "update view";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);    // QApplication::applicationDirPath() + "CONFIG.db"     不能包含字符
//     db.setUserName("admin");
//     db.setPassword("admin");
    if (!db.open())     // if (!db.open("admin","admin"))
    {
        qDebug() << "打开数据库文件失败！";
        return;
    }
    QSqlQueryModel sqlModel;
    QString   strQuery = "select count(*) from stm32_data";        // select count(*) from table
    sqlModel.setQuery(strQuery);
    qint64 dataCount = sqlModel.record(0).value(0).toLongLong();
    if(dataCount < 10000)
    {
        qDebug() << "数据文件数据量太少！";
        return;
    }
    zoomIndexMin = 1;
    zoomIndexMax = dataCount;
    zoomIndex = zoomIndexMax / 2;              // 默认
    zoomMagnifyActual = 1;      // 默认放大1倍
    zoomMagnifyMax = dataCount / 10000;     // 计算最大能够放大的倍数
    qDebug()<<zoomIndex<<zoomMagnifyActual<<zoomMagnifyMax;
    zoomSlide->setEnabled(true);
    zoomSlide->setValue(zoomMagnifyActual);     // 设置滑条值为1
    zoomSlide->setMinimum(1);  // 最小值
    zoomSlide->setMaximum(zoomMagnifyMax);  // 最大值
    zoomSlide->setSingleStep(1); // 步长
    zoomSlide->setPageStep(zoomMagnifyMax / 10);      // 页步长
    zoomSlide->setTickInterval(zoomMagnifyMax / 10);  // 设置刻度间隔
    unitVol->setText("电压:V");
    unitCur->setText("电流:mA");

    UpdateZoomKeyEnable();
    UpdateChartData();
}

void HistoryView::wheelEvent(QWheelEvent *event)
{
    if(zoomIndex == 0)
        return;
    int numDegrees = event->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
    int numSteps = numDegrees / 15;//滚动的步数，*15就是鼠标滚动的角度
    numSteps *= 5;       // 每一格缩放5倍

    if(zoomMagnifyActual == zoomMagnifyMax && numSteps > 0)
        return;
    if(zoomMagnifyActual == 1 && numSteps < 0)
        return;

    if(zoomMagnifyActual + numSteps > zoomMagnifyMax)
        zoomMagnifyActual = zoomMagnifyMax;
    if(zoomMagnifyActual + numSteps < 1)
        zoomMagnifyActual = 1;


//    if (event->orientation() == Qt::Horizontal) {
////       scrollHorizontally(numSteps);       //水平滚动
//        qDebug()<<"Horizontal：numDegrees numSteps = "<<numDegrees<<numSteps;
//    } else {
////       scrollVertically(numSteps);       //垂直滚动
//        qDebug()<<"Vertical：numDegrees numSteps = "<<numDegrees<<numSteps;
//    }
    zoomMagnifyActual += numSteps;
    zoomSlide->setValue(zoomMagnifyActual);
    UpdateZoomKeyEnable();

    event->accept();      //接收该事件
}

void HistoryView::ZoomSlideValueChanged(int value)
{
    qDebug()<<"ZoomSlideValueChanged value = "<<value;
    zoomMagnifyActual = value;
    UpdateZoomKeyEnable();
    UpdateChartData();
}

void HistoryView::ClickZoomX2()
{
    zoomMagnifyActual += 1;
    zoomSlide->setValue(zoomMagnifyActual);
    UpdateZoomKeyEnable();
}

void HistoryView::ClickZoomD2()
{
    zoomMagnifyActual -= 1;
    zoomSlide->setValue(zoomMagnifyActual);
    UpdateZoomKeyEnable();
}

void HistoryView::UpdateZoomKeyEnable()
{
    if(zoomMagnifyActual + 1 > zoomMagnifyMax)
        zoomX2->setEnabled(false);
    else
        zoomX2->setEnabled(true);

    if(zoomMagnifyActual - 1 < 1)
        zoomD2->setEnabled(false);
    else
        zoomD2->setEnabled(true);
}

void HistoryView::UpdateMarkLine(qlonglong index, double vol, double cur)
{

    chatCurrent->removeSeries(seriesMarkA);
    seriesMarkA->removePoints(0, seriesMarkA->count());
    seriesMarkA->append(index, 0);
    seriesMarkA->append(index, axisCurrentY->max());
    chatCurrent->addSeries(seriesMarkA);
    seriesMarkA->attachAxis(axisCurrentX);
    seriesMarkA->attachAxis(axisCurrentY);

    chatVoltage->removeSeries(seriesMarkV);
    seriesMarkV->removePoints(0, seriesMarkV->count());
    seriesMarkV->append(index, 0);
    seriesMarkV->append(index, axisVoltageY->max());
    chatVoltage->addSeries(seriesMarkV);
    seriesMarkV->attachAxis(axisVoltageX);
    seriesMarkV->attachAxis(axisVoltageY);

    if(cur < 1)
        markLabel->setText(QDateTime::fromMSecsSinceEpoch(index).toString("yyyy-MM-dd hh:mm:ss") + " 电压:" + QString::number(vol, 'f', 3) + "V 电流:"  + QString::number(cur * 1000, 'f', 3) + "uA");
    else if(cur >= 1 && cur < 1000)
         markLabel->setText(QDateTime::fromMSecsSinceEpoch(index).toString("yyyy-MM-dd hh:mm:ss") + " 电压:" + QString::number(vol, 'f', 3) + "V 电流:"  + QString::number(cur, 'f', 3) + "mA");
    else if(cur >= 1000)
         markLabel->setText(QDateTime::fromMSecsSinceEpoch(index).toString("yyyy-MM-dd hh:mm:ss") + " 电压:" + QString::number(vol, 'f', 3) + "V 电流:"  + QString::number(cur, 'f', 2) + "mA");


}

void HistoryView::UpdateChartData()
{
    qint64 sqlIdMin = zoomIndex - (zoomMagnifyMax + 1 - zoomMagnifyActual) * 5000;
    qint64 sqlIdMax = zoomIndex + (zoomMagnifyMax + 1 - zoomMagnifyActual) * 5000;
    if(sqlIdMin < zoomIndexMin)
    {
        sqlIdMin = zoomIndexMin;
        sqlIdMax = sqlIdMin + (zoomMagnifyMax + 1 - zoomMagnifyActual) * 10000;
    }
    if(sqlIdMax > zoomIndexMax)
    {
        sqlIdMax = zoomIndexMax;
        sqlIdMin = sqlIdMax - (zoomMagnifyMax + 1 - zoomMagnifyActual) * 10000;
    }
    qDebug()<<"zoomMagnifyActual = "<<zoomMagnifyActual<< " zoomMagnifyMax = "<< zoomMagnifyMax;
    qDebug()<<"sqlIdMin = "<<sqlIdMin<< " sqlIdMax = "<< sqlIdMax;
    QSqlQueryModel sqlModel;
    QString strQuery = "select * from stm32_data where (id % " + QString::number(zoomMagnifyMax + 1 - zoomMagnifyActual, 10)
            + " == 0) and id >= " + QString::number(sqlIdMin, 10) + " and id <= " + QString::number(sqlIdMax, 10) + " ";
    sqlModel.setQuery(strQuery);
    while(sqlModel.canFetchMore())
    {
        sqlModel.fetchMore();
    }
    qDebug()<< "Update Chart Data:" <<sqlModel.rowCount();
    chatCurrent->removeSeries(seriesCurrent);
    chatVoltage->removeSeries(seriesVoltage);
    seriesCurrent->removePoints(0, seriesCurrent->points().count());
    seriesVoltage->removePoints(0, seriesVoltage->points().count());
//    QLineSeries *newSeries = new QLineSeries();
//    qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch();
//    qDebug()<<sqlModel.record(0).value("time").toLongLong();
//    qDebug()<<sqlModel.record(0).value("time").toDateTime().toMSecsSinceEpoch();
//    qDebug()<<sqlModel.record(0).value("current").toDouble();
    bool notGetIndex = true;
    qlonglong index = 0;
    double vol = 0;
    double cur = 0;
    for(int nProvinceNum = 0; nProvinceNum < sqlModel.rowCount(); nProvinceNum++)
    {
        seriesCurrent->append(sqlModel.record(nProvinceNum).value("time").toLongLong(), sqlModel.record(nProvinceNum).value("current").toDouble());      //
        seriesVoltage->append(sqlModel.record(nProvinceNum).value("time").toLongLong(), sqlModel.record(nProvinceNum).value("voltage").toDouble());
//         newSeries->append(sqlModel.record(nProvinceNum).value("time").toLongLong(), sqlModel.record(nProvinceNum).value("current").toDouble());
        if(zoomIndex <= sqlModel.record(nProvinceNum).value("id").toLongLong() && notGetIndex)  // 获取索引id对应的时间
        {
            notGetIndex = false;
            index = sqlModel.record(nProvinceNum).value("time").toLongLong();
            vol = sqlModel.record(nProvinceNum).value("voltage").toDouble();
            cur = sqlModel.record(nProvinceNum).value("current").toDouble();
        }
        if(zoomIndex == zoomIndexMax && nProvinceNum == sqlModel.rowCount() - 1)    // 如果索引id为最大值，获取索引id对应的时间
        {
            index = sqlModel.record(nProvinceNum).value("time").toLongLong();
            vol = sqlModel.record(nProvinceNum).value("voltage").toDouble();
            cur = sqlModel.record(nProvinceNum).value("current").toDouble();
        }
    }
//    for(int i = 0; i < 1000; i++)
//    {
//        seriesCurrent->append(QDateTime::currentDateTime().toMSecsSinceEpoch() + i, i);
//    }
//    seriesCurrent->append(QDateTime::currentDateTime().toMSecsSinceEpoch(), 2000);
//    qDebug()<<seriesCurrent->count();
//    qDebug()<<newSeries->count();
    chatCurrent->addSeries(seriesCurrent);
    chatVoltage->addSeries(seriesVoltage);
//    chatCurrent->addSeries(newSeries);

//    chatCurrent->removeAxis(axisCurrentX);
//    chatCurrent->removeAxis(axisCurrentY);
    axisCurrentX->setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
    axisCurrentY->setRange(0, 0);
//    qDebug()<<axisCurrentY->min();
//    qDebug()<<axisCurrentY->max();
    seriesCurrent->attachAxis(axisCurrentX);
    seriesCurrent->attachAxis(axisCurrentY);
//    qDebug()<<axisCurrentY->min();
//    qDebug()<<axisCurrentY->max();
    if(axisCurrentY->min() == axisCurrentY->max())
        axisCurrentY->setRange(0, axisCurrentY->max() * 1.2);
//    axisCurrentX = new QDateTimeAxis;
//    axisCurrentX->setTickCount(10);
//    axisCurrentX->setFormat("hh:mm:ss");
//    axisCurrentX->setTitleText("Date");
//    chatCurrent->addAxis(axisCurrentX, Qt::AlignBottom);
//    seriesCurrent->attachAxis(axisCurrentX);

//    QValueAxis *tAxisCurrentY = new QValueAxis;
//    axisCurrentY->setLabelFormat("%.2f");
//    axisCurrentY->setTitleText("Sunspots count");
//    chatCurrent->addAxis(axisCurrentY, Qt::AlignLeft);
//    seriesCurrent->attachAxis(axisCurrentY);

    axisVoltageX->setRange(QDateTime::fromMSecsSinceEpoch(0), QDateTime::fromMSecsSinceEpoch(0));
    axisVoltageY->setRange(0, 0);
    seriesVoltage->attachAxis(axisVoltageX);
    seriesVoltage->attachAxis(axisVoltageY);
    if(axisVoltageY->min() == axisVoltageY->max())
        axisVoltageY->setRange(0, axisVoltageY->max() * 1.2);


//    qlonglong index = seriesCurrent->points()[seriesCurrent->points().count() / 2].x();
    qDebug() << "index = " << index << QDateTime::fromMSecsSinceEpoch(index);
    UpdateMarkLine(index, vol, cur);

    chatCurrent->update();
    chatVoltage->update();
    scene()->update();
}
