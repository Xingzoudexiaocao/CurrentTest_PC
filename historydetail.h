#ifndef HISTORYDETAIL_H
#define HISTORYDETAIL_H

#include <QObject>
#include <QWidget>
#include "comdata.h"
#include <QtDebug>
#include <QtWidgets/QGraphicsView>
#include <QScrollBar>
//#include <QtCharts/QChartGlobal>
//#include <QtCharts/QtCharts>
//#include <QtCharts/QChartView>
//#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
//#include <QtCharts/QDateTimeAxis>
//#include <QtCharts/QValueAxis>
#include "qchartviewer.h"
#include "about.h"
#include "usb_hid.h"
#include <math.h>
#include <vector>
#include <sstream>

class HistoryDetail : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HistoryDetail(QWidget *parent = nullptr);
    ~HistoryDetail();

    void LoadingData(QString fileName);
    void ClearData(void);
signals:

private:
    unsigned long long DataSize;
    double *d_timeStamps;	// The timestamps for the data series
    double *d_dataSeriesV;	// The values for the data series A
    double *d_dataSeriesA;	// The values for the data series B
//    double d_time[100000];
//    double d_V[100000];
//    double d_A[100000];

    QChartViewer *m_ChartViewer;        // QChartViewer control
    QChartViewer *m_ChartViewer_2;        // QChartViewer control
    QScrollBar *m_HScrollBar;           // The scroll bar
    QScrollBar *m_HScrollBar_2;           // The scroll bar

//    void drawChart(QChartViewer *viewer, int index);           // Draw chart
//    void trackLineLabel(XYChart *c, int mouseX, int index);    // Draw track cursor
//    void updateControls(QChartViewer *viewer, QScrollBar *bar);      // Update other controls as viewport changes
private slots:
//    void updateChart();                 // Update the chart.
//    void onMouseMovePlotArea(QMouseEvent *);
//    void onMouseMovePlotArea_2(QMouseEvent *);
//    void onMouseUsageChanged(int mouseUsage);       // Pointer/zoom in/zoom out button clicked
//    void onHScrollBarChanged(int value);            // Scrollbar changed
//    void onHScrollBarChanged_2(int value);            // Scrollbar changed
//    void onViewPortChanged();                       // Viewport has changed
//    void onViewPortChanged_2();                       // Viewport has changed
//    void onChartUpdateTimer(QChartViewer *viewer);                      // Update the chart.

public slots:
    void ReceiveTest(void);
    void UpdateChartData(void);
};

#endif // HISTORYDETAIL_H
