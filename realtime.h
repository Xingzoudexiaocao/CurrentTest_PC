#ifndef REALTIME_H
#define REALTIME_H

#include <QDialog>
#include <QDateTime>
#include <QTimer>
#include <QLabel>
#include <QProgressBar>
#include <QButtonGroup>
#include <QIcon>
#include <QPushButton>
#include <QComboBox>
#include "qchartviewer.h"
#include <QTextCodec>
#include <QScrollBar>
#include <QFileDialog>
#include <QPixmap>
#include <QFont>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include "comdata.h"
#include "usb_hid.h"
#include "usb_receive_thread.h"
#include "usb_send_thread.h"
#include "about.h"
#include "historyview.h"
#include "historydetail.h"
#include <QSpinBox>
#include <QLineEdit>
#include "sqlite_write_thread.h"
#include "averagesubframe.h"
#include "calculate_tread.h"

class RealTime : public QWidget
{
    Q_OBJECT
public:
    explicit RealTime(QWidget *parent = nullptr, ComData *comD = nullptr,  USB_HID *hid = nullptr);
    ~RealTime();

    USB_Receive_Thread *m_UsbReceiveThread;
    USB_Send_Thread *m_UsbSendThread;
    Sqlite_Write_Thread *m_SqliteThread;
    Calculate_Tread *m_CalculateThread;

signals:
    void CreateSqilite(void);
    void SignalsTest(void);

    void singalCurUpdateT1AndT2(qint8, qint64);
private:
    // The initial full range is set to 60 seconds of data.
    static const int initialFullRange = 60;

    // The maximum zoom in is 10 seconds.
    static const int zoomInLimit = 1;

    QFont    font;
    QFont    font_2;
    QDateTime m_nextDataTime;           // Used by the random number generator to generate realtime data.

    QLabel *m_ValueB;                   // Label to display the realtime value B
    QLabel *m_ValueC;                   // Label to display the realtime value C
    QLabel *m_Power;
    QLabel *m_Energy;
    QLabel *m_unitA;    // 电流单位
    QLabel *m_averageA;    // 电流单位
    QLabel *buf1_QL;    // 平均值显示
    QLabel *buf2_QL;
    QLabel *buf3_QL;
    QLabel *usb_str1;
    QLabel *usb_str2;
    QLabel *usb_str3;
    QLabel *m_Temp;
    QLabel *m_Tips;
    QLabel *m_Error;

    QFrame *frame_2_updata;
    QTabWidget * tabWidget;     // Tab界面
    QTableView *dataView;       // TableView数据
    QStandardItemModel *dataModel;     // TableModel数据
    About *m_About;
    int cntDisplay;


    QChartViewer *m_ChartViewer;        // QChartViewer control
    QChartViewer *m_ChartViewer_2;        // QChartViewer control
    QTimer *m_ChartUpdateTimer;         // The chart update timer
    QScrollBar *m_HScrollBar;           // The scroll bar
    QScrollBar *m_HScrollBar_2;           // The scroll bar

    QPushButton *connectUSB;
    QPushButton *disconnectUSB;

    QPushButton *play;
    QPushButton *pause;
    QPushButton *download;

    QLabel *appVersion;
    QLabel *appLength;
    QPushButton *updataFile;
    QPushButton *updataOpen;
    QPushButton *updataBegin;
    QProgressBar *updataBar;
    QLabel *updataTips;

    ComData *m_ComData;
    USB_HID *m_UsbHid;

    HistoryView *historyView;
    HistoryDetail *historyDetail;

    QSpinBox *averageValue;
    QSpinBox *batteryCapacity_2;
    QSpinBox *batteryCapacity;
    QDoubleSpinBox *batteryVoltageBegin;
    QDoubleSpinBox *batteryVoltageEnd;

    QLabel *bTotalCap;          // 总容量
    QLabel *bRemainCap;         // 剩余容量
    QLabel *bRunningTimeHour;       // 运行时间
    QLabel *bRunningTimeMinute;       // 运行时间
    QLabel *bRemainTimeHour;        // 剩余时间
    QLabel *bRemainTimeMinute;        // 剩余时间
    QLabel *bDifferCap;          // 总容量

    QTimer *SendVerifyCmd;       // 发送校验字节
    QTimer *SendRandomKeyCmd;       // 发送密钥字节
    qint16 SendKeyCount;     // 发送密钥字节计数

    QLabel *sDeviceStates;  // 设备状态
    QLabel *sRemainTime;    // 运行时间

    QComboBox *FixCurrentScale;
    double fixCurrentValue;

    QPushButton *m_SubButton_Cur;
    AverageSubFrame *m_SubFrame_Cur;

    void drawChart_Current(void);
    void drawChart_Voltage(void);
    void drawChart(QChartViewer *viewer, int index);           // Draw chart
    void trackLineLabel(XYChart *c, int mouseX, int index);    // Draw track cursor
    void trackLineLabel_T1Or2(XYChart *c, int mouseX, int index, int T1Or2);
    void updateControls(QChartViewer *viewer, QScrollBar *bar);      // Update other controls as viewport changes
private slots:
//    void onUpdatePeriodChanged(QString);// The chart update timer interval has changed.
    QString loadFontFamilyFromTTF(QString);
    void linkUs(QString);
    void getData();                     // Get new data values
    void updateChart();                 // Update the chart.
    void onMouseClick(QMouseEvent *);
    void onMouseClick_2(QMouseEvent *);
    void onMouseMovePlotArea(QMouseEvent *);
    void onMouseMovePlotArea_2(QMouseEvent *);
    void onMouseUsageChanged(int mouseUsage);       // Pointer/zoom in/zoom out button clicked
    void onSave(bool);                              // Save button clicked
    void onHScrollBarChanged(int value);            // Scrollbar changed
    void onHScrollBarChanged_2(int value);            // Scrollbar changed
    void onViewPortChanged();                       // Viewport has changed
    void onViewPortChanged_2();                       // Viewport has changed
    void onChartUpdateTimer(QChartViewer *viewer);                      // Update the chart.
    void onConnectUSB();    // 连接USB_HID方法
    void onDisConnectUSB();    // 关闭连接USB_HID方法
//    void onSendUSB();    // 发送数据方法
//    void onReadUSB();    // 接收数据方法
    void CreateData();
    void showVAW(double v, double mA);
    void onBtnPlay();
    void onBtnPause();
    void onBtnDownload();
    QString doubleToTime(double dTime);
    void UpdataOpen();
    void UpdataSend();

    void slotAverageValue(int);
    void slotBatteryValue_2(int);
    void slotBatteryValue(int);

    void slotDifferVBegin(double);
    void slotDifferVEnd(double);

    void onSettingBtn(void);

    void slotSendVerifyCmd(void);
    void slotSendRandomKeyCmd(void);

    void slotFixCurrentScale(int);

    void slotSubButtonCurrent(void);

public slots:
    void m_get_USB_Data(QDateTime, double, unsigned char, unsigned char);
    void m_get_Version_Length(unsigned long long, unsigned long long);
    void thread_receive_finished();
    void thread_send_finished();
    void thread_sqlite_finished();
    void aboutClose(void);

    void updataProgressBar(unsigned long);
    void upadataSuccess();
    void updataFail();
    void upadtaTimeOut();

    void send_CMD(unsigned char cmd);
//    void writeSQL(qint64 time, double vol, double cur);

    void slotQBoxTip(QString);

    void slotShowDifferVoltage(void);
    void slotShowEnergy(void);
    void showAverage(void);
    void slotShowTime(qint64 runningT, qint64 remainT);

    void slotUpdateWorkingTime(qint64, qint64, qint64);

    void slotVerifySuccess(void);
    void slotRandomKeySuccess(void);

protected:
//    virtual void resizeEvent(QResizeEvent *event);
};

#endif // REALTIME_H
