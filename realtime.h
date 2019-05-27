#ifndef REALTIME_H
#define REALTIME_H

#include <QDialog>
#include <QDateTime>
#include <QTimer>
#include <QLabel>
#include <QButtonGroup>
#include <QIcon>
#include <QPushButton>
#include <QComboBox>
#include "qchartviewer.h"
#include <QTextCodec>
#include <QScrollBar>
#include <QFileDialog>
#include "comdata.h"
#include "usb_hid.h"
#include "usb_receive_thread.h"

class RealTime : public QWidget
{
    Q_OBJECT
public:
    explicit RealTime(QWidget *parent = nullptr, ComData *comD = nullptr);
    ~RealTime();

private:
    // The initial full range is set to 60 seconds of data.
    static const int initialFullRange = 60;

    // The maximum zoom in is 10 seconds.
    static const int zoomInLimit = 10;

    QDateTime m_nextDataTime;           // Used by the random number generator to generate realtime data.

    QLabel *m_ValueB;                   // Label to display the realtime value B
    QLabel *m_ValueC;                   // Label to display the realtime value C

    QChartViewer *m_ChartViewer;        // QChartViewer control
    QChartViewer *m_ChartViewer_2;        // QChartViewer control
    QTimer *dataRateTimer;
    QTimer *m_ChartUpdateTimer;         // The chart update timer
    QScrollBar *m_HScrollBar;           // The scroll bar
    QScrollBar *m_HScrollBar_2;           // The scroll bar

    QPushButton *connectUSB;
    QPushButton *disconnectUSB;

    ComData *m_ComData;
    USB_HID *m_UsbHid;
    USB_Receive_Thread *m_UsbReceiveThread;

    void drawChart(QChartViewer *viewer, int index);           // Draw chart
    void trackLineLabel(XYChart *c, int mouseX);    // Draw track cursor
    void updateControls(QChartViewer *viewer, QScrollBar *bar);      // Update other controls as viewport changes

private slots:
//    void onUpdatePeriodChanged(QString);// The chart update timer interval has changed.
    void getData();                     // Get new data values
    void updateChart();                 // Update the chart.
    void drawChart();                   // Draw the chart.
    void drawChart_2();                   // Draw the chart.
    void onMouseMovePlotArea(QMouseEvent *);
    void onMouseMovePlotArea_2(QMouseEvent *);
    void trackLineLegend(XYChart *c, int mouseX);
    void onMouseUsageChanged(int mouseUsage);       // Pointer/zoom in/zoom out button clicked
    void onSave(bool);                              // Save button clicked
    void onHScrollBarChanged(int value);            // Scrollbar changed
    void onHScrollBarChanged_2(int value);            // Scrollbar changed
    void onViewPortChanged();                       // Viewport has changed
    void onViewPortChanged_2();                       // Viewport has changed
    void onChartUpdateTimer(QChartViewer *viewer);                      // Update the chart.
    void onConnectUSB();    // 连接USB_HID方法
    void onDisConnectUSB();    // 关闭连接USB_HID方法
    void onSendUSB();    // 发送数据方法
    void onReadUSB();    // 接收数据方法
    void CreateData();
public slots:
    void m_get_USB_Data(unsigned char *buf, unsigned char len);
    void thread_finished();
};

#endif // REALTIME_H
