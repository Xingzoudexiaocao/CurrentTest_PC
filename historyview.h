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
    void UptateChartVoltage(void);
    void UptateChartCurrent(void);

protected:
    void mousePressEvent(QMouseEvent *event);

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

signals:

public slots:
};

#endif // HISTORYVIEW_H
