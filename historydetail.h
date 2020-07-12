#ifndef HISTORYDETAIL_H
#define HISTORYDETAIL_H

#include <QObject>
#include <QWidget>
#include "comdata.h"
#include <QtDebug>
#include <QtWidgets/QGraphicsView>
#include <QScrollBar>
#include <QPushButton>
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
#include <QSlider>
#include "doubleslider.h"
#include <QFontDatabase>
#include <QFileDialog>
#include <QApplication>

#include <algorithm>
using namespace std;      //这一句也不能少


class HistoryDetail : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HistoryDetail(QWidget *parent = nullptr, USB_HID *hid = nullptr, ComData *comD = nullptr);
    ~HistoryDetail();

    void LoadingData(QString fileName);
    void ClearData(void);
signals:

private:
    HISTORY_DATAINFO_STRUCT m_Current_DataInfo;
    HISTORY_DATAINFO_STRUCT m_Voltage_DataInfo;
    HISTORY_DATAINFO_STRUCT m_Power_DataInfo;
    QLabel *m_Currnet_Avg;
    QLabel *m_Currnet_Max;
    QLabel *m_Currnet_Min;
    QLabel *m_Voltage_Avg;
    QLabel *m_Voltage_Max;
    QLabel *m_Voltage_Min;
    QLabel *m_Power_Avg;
    QLabel *m_Power_Max;
    QLabel *m_Power_Min;

    float DifferValue;         // maxValue和minValue的差值，若有变化则更新UI
    qint64 BeginDateTime;       // 数据库起始时间
    qint64 CountSize;           // 数据库数据个数总和
    qint64 SelectSize;           // 实际显示的数据个数总和
    qint64 IntervalValue;       // 波形上相邻的两个点间隔的数据个数 = SelectSize/DataSize + 1
    QLabel *BeginT;         // 显示起始时间
    QLabel *LastT;          // 显示结束时间
    QLabel *BeginSelectT;         // 显示起始时间
    QLabel *LastSelectT;          // 显示结束时间

    QFont    font;
    QPushButton *historyFile;
    QPushButton *historyOpen;

    ComData *m_ComData;
    USB_HID *m_UsbHid;

    QPushButton *m_EjectSubButton;
    QFrame *m_SubFrame;
    DoubleSlider *m_DoubleSlider;

    qint64 DataSize;
    double *d_timeStamps;	// The timestamps for the data series
    double *d_dataSeriesV;	// The values for the data series A
    double *d_dataSeriesA;	// The values for the data series B
    qint64 d_currentIndex;
//    double d_time[100000];
//    double d_V[100000];
//    double d_A[100000];

    QChartViewer *m_ChartViewer;        // QChartViewer control
    QChartViewer *m_ChartViewer_2;        // QChartViewer control
    QScrollBar *m_HScrollBar;           // The scroll bar
    QScrollBar *m_HScrollBar_2;           // The scroll bar

    void drawChart(QChartViewer *viewer, int index);           // Draw chart
    void trackLineLabel(XYChart *c, int mouseX, int index);    // Draw track cursor
    void updateControls(QChartViewer *viewer, QScrollBar *bar);      // Update other controls as viewport changes
private slots:
    void updateChart();                 // Update the chart.
    void onMouseMovePlotArea(QMouseEvent *);
    void onMouseMovePlotArea_2(QMouseEvent *);
    void onMouseUsageChanged(int mouseUsage);       // Pointer/zoom in/zoom out button clicked
    void onHScrollBarChanged(int value);            // Scrollbar changed
    void onHScrollBarChanged_2(int value);            // Scrollbar changed
    void onViewPortChanged();                       // Viewport has changed
    void onViewPortChanged_2();                       // Viewport has changed
    void onChartUpdateTimer(QChartViewer *viewer);                      // Update the chart.

    void slotSubButtonClick(void);
    void slotHistoryOpen(void);

    void initDisplay(void);     // 显示初始化
    void loadingDataByTime(qint64, qint64);
    void updateInfomationDisplay(void);
public slots:
    void ReceiveTest(void);
    void UpdateChartData(void);
    void mouseReleaseSlot(void);
};

#endif // HISTORYDETAIL_H
