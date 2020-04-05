#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QObject>
#include <QWidget>
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include "comdata.h"
#include <QWheelEvent>
#include <QLCDNumber>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QMouseEvent;
class QResizeEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE
class Callout;

QT_CHARTS_USE_NAMESPACE

class HistoryView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HistoryView(QWidget *parent = nullptr);
    void LoadingData(QString fileName);
    void ClearData(void);

protected:
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public:
    QLineSeries *seriesVoltage;
    QChart *chatVoltage;
//    QChartView *chartViewVoltage;
    QDateTimeAxis *axisVoltageX;
    QValueAxis *axisVoltageY;
    QLineSeries *seriesCurrent;
    QChart *chatCurrent;
//    QChartView *chartViewCurrent;
    QDateTimeAxis *axisCurrentX;
    QValueAxis *axisCurrentY;

    QLineSeries *seriesMarkV;
    QLineSeries *seriesMarkA;

    QPushButton *zoomX2;        // 缩放按键
    QPushButton *zoomD2;

    qint64 zoomIndex;               // 当前选中点索引值
    qint64 zoomIndexMin;               // 当前选中点最小索引值
    qint64 zoomIndexMax;               // 当前选中点最大索引值
    qint64 zoomMagnifyActual;       // 图像实际放大倍数
    qint64 zoomMagnifyMax;          // 图像最大可以放大的倍数

//    QLCDNumber *testLCD;
    QSlider *zoomSlide;
    QLabel *unitVol;
    QLabel *unitCur;
    QLabel *markLabel;

signals:

public slots:
    void ClickZoomX2();
    void ClickZoomD2();

    void UpdateZoomKeyEnable();
    void UpdateMarkLine(qlonglong index, double vol, double cur);
    void UpdateChartData();
    void ZoomSlideValueChanged(int value);

};

#endif // HISTORYVIEW_H
