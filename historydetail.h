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
#include <QRadioButton>
#include <QComboBox>

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
    double fixCurrentValue;
    qint8 keyValue;         // T1/T2标志位：1=T1；2=T2；
    QRadioButton *T1_Text;
    QRadioButton *T2_Text;
    QLabel *T2_T1_Label;
    QLabel *T2_T1_Currnet_Avg;
    QLabel *T2_T1_Currnet_Max;
    QLabel *T2_T1_Currnet_Min;
    QLabel *T2_T1_Voltage_Avg;
    QLabel *T2_T1_Voltage_Max;
    QLabel *T2_T1_Voltage_Min;
    HISTORY_DATAINFO_STRUCT T2_T1_Current_DataInfo;
    HISTORY_DATAINFO_STRUCT T2_T1_Voltage_DataInfo;

    qint64 T1_Index;        // 波形T1起始索引
    qint64 T2_Index;        // 波形T2起始索引
    qint64 T1_DateTime;        // 波形T1对应的时间
    qint64 T2_DateTime;        // 波形T2对应的时间
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

    QPushButton *ZoomMax;
    QPushButton *ZoomMin;

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
    void slotButtonT1Check(bool);
    void slotButtonT2Check(bool);
    void UpdateT1T2Display(void);
    void getT1T2DataFromSqlite(qint64, qint64);
    void slotFixCurrentScale(int);

    void updateChart();                 // Update the chart.
    void onMouseClick(QMouseEvent *);
    void onMouseClick_2(QMouseEvent *);
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

    void slotZoomMaxClick(void);
    void slotZoomMinClick(void);

    void initDisplay(void);     // 显示初始化
    void loadingDataByTime(qint64, qint64);
    void updateInfomationDisplay(void);

    QString doubleToTime(double dTime);
public slots:
    void ReceiveTest(void);
    void mouseReleaseSlot(void);
};

#endif // HISTORYDETAIL_H
