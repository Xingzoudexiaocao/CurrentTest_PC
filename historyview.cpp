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
}

void HistoryView::mousePressEvent(QMouseEvent *event)
{
//    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
//    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    qlonglong m_x_V, m_x_A;

    qDebug() << "click chart view" << event->pos();
//    chatVoltage->removeSeries(seriesMarkV);

    if(seriesVoltage->points().count() <= 0 || seriesCurrent->points().count() <= 0)    //
        return;

//    qDebug() << "pos[0] = " << seriesCurrent->points()[0].x();
//    qDebug() << "pos[count-1] = " << seriesCurrent->points()[seriesCurrent->points().count()-1].x();

//    m_x_V = chatVoltage->mapToValue(event->pos()).x();
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

//    if(dec1 > 0 && dec2 < 0)
//    {
        qDebug() << "m_x_A = " << m_x_A << QDateTime::fromMSecsSinceEpoch(m_x_A);
        chatCurrent->removeSeries(seriesMarkA);
        seriesMarkA->removePoints(0, seriesMarkA->count());
//        *seriesMarkA<<QPoint(pos_x_min.toMSecsSinceEpoch(), 0)<<QPoint(m_x_A, 0)<<QPoint(m_x_A, 100)<<QPoint(pos_x_max.toMSecsSinceEpoch(), 100);
        seriesMarkA->append(m_x_A, 0);
        seriesMarkA->append(m_x_A, axisCurrentY->max());
        chatCurrent->addSeries(seriesMarkA);
        seriesMarkA->attachAxis(axisCurrentX);
        seriesMarkA->attachAxis(axisCurrentY);
//    }

        chatVoltage->removeSeries(seriesMarkV);
        seriesMarkV->removePoints(0, seriesMarkV->count());
//        *seriesMarkV<<QPoint(pos_x_min.toMSecsSinceEpoch(), 0)<<QPoint(m_x_A, 0)<<QPoint(m_x_A, 100)<<QPoint(pos_x_max.toMSecsSinceEpoch(), 100);
        seriesMarkV->append(m_x_A, 0);
        seriesMarkV->append(m_x_A, axisVoltageY->max());
        chatVoltage->addSeries(seriesMarkV);
        seriesMarkV->attachAxis(axisVoltageX);
        seriesMarkV->attachAxis(axisVoltageY);


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
    QString   strQuery = "select * from stm32_data";
    sqlModel.setQuery(strQuery);
    while(sqlModel.canFetchMore())
    {
        sqlModel.fetchMore();
    }
    qDebug()<<sqlModel.rowCount();
    chatCurrent->removeSeries(seriesCurrent);
    chatVoltage->removeSeries(seriesVoltage);
    seriesCurrent->removePoints(0, seriesCurrent->points().count());
    seriesVoltage->removePoints(0, seriesVoltage->points().count());
//    QLineSeries *newSeries = new QLineSeries();
//    qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch();
//    qDebug()<<sqlModel.record(0).value("time").toLongLong();
//    qDebug()<<sqlModel.record(0).value("time").toDateTime().toMSecsSinceEpoch();
//    qDebug()<<sqlModel.record(0).value("current").toDouble();
    for(int nProvinceNum = 0; nProvinceNum < sqlModel.rowCount(); nProvinceNum++)
    {
        seriesCurrent->append(sqlModel.record(nProvinceNum).value("time").toLongLong(), sqlModel.record(nProvinceNum).value("current").toDouble());      //
        seriesVoltage->append(sqlModel.record(nProvinceNum).value("time").toLongLong(), sqlModel.record(nProvinceNum).value("voltage").toDouble());
//         newSeries->append(sqlModel.record(nProvinceNum).value("time").toLongLong(), sqlModel.record(nProvinceNum).value("current").toDouble());
//        qDebug()<<sqlModel.record(nProvinceNum).value("time");
//        qDebug()<<sqlModel.record(nProvinceNum).value("voltage");
//        qDebug()<<sqlModel.record(nProvinceNum).value("current");
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
    qDebug()<<axisCurrentY->min();
    qDebug()<<axisCurrentY->max();
    seriesCurrent->attachAxis(axisCurrentX);
    seriesCurrent->attachAxis(axisCurrentY);
    qDebug()<<axisCurrentY->min();
    qDebug()<<axisCurrentY->max();
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

    chatCurrent->update();
    chatVoltage->update();
    scene()->update();
}
void HistoryView::UptateChartVoltage(void)
{

}
void HistoryView::UptateChartCurrent(void)
{

}
