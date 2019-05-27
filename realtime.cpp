#include "realtime.h"
#include <math.h>
#include <vector>
#include <sstream>
#include <QDebug>
#include "mainwindow.h"

using namespace std;

static const int DataInterval = 1;

RealTime::RealTime(QWidget *parent, ComData *comD) : QWidget(parent)
{
    qDebug() << "RealTime构造函数。";
    m_ComData = comD;
    m_UsbHid = new USB_HID(comD);

    setFixedSize(990, 850);
////    setMinimumSize(200,200);
//    setWindowTitle("Simple Realtime Chart");

    // The frame on the left side
    QFrame *frame = new QFrame(this);
    frame->setGeometry(4, 4, 150, 820);
    frame->setFrameShape(QFrame::StyledPanel);

    // Run push button
    connectUSB = new QPushButton(QIcon(":/play.png"), "连接设备", frame);
    connectUSB->setGeometry(4, 8, 142, 40);
    connectUSB->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    connect(connectUSB, SIGNAL(clicked(bool)), SLOT(onConnectUSB()));
    // Run push button
    disconnectUSB = new QPushButton(QIcon(":/pause.png"), "关闭设备", frame);
    disconnectUSB->setGeometry(4, 48, 142, 40);
    disconnectUSB->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:24px;}");
    disconnectUSB->setEnabled(false);
    connect(disconnectUSB, SIGNAL(clicked(bool)), SLOT(onDisConnectUSB()));

//    // Pointer push button
//    QPushButton *pointerPB = new QPushButton(QIcon(":/pointer.png"), "Pointer", frame);
//    pointerPB->setGeometry(4, 64, 112, 28);
//    pointerPB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
//    pointerPB->setCheckable(true);

//    // Zoom In push button
//    QPushButton *zoomInPB = new QPushButton(QIcon(":/zoomin.png"), "Zoom In", frame);
//    zoomInPB->setGeometry(4, 92, 112, 28);
//    zoomInPB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
//    zoomInPB->setCheckable(true);

//    // Zoom Out push button
//    QPushButton *zoomOutPB = new QPushButton(QIcon(":/zoomout.png"), "Zoom Out", frame);
//    zoomOutPB->setGeometry(4, 120, 112, 28);
//    zoomOutPB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
//    zoomOutPB->setCheckable(true);

//    // Save push button
//    QPushButton *savePB = new QPushButton(QIcon(":/save.png"), "Save", frame);
//    savePB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
//    savePB->setGeometry(4, 148, 112, 28);
//    connect(savePB, SIGNAL(clicked(bool)), SLOT(onSave(bool)));

//    // The Pointer/Zoom In/Zoom Out buttons form a button group
//    QButtonGroup *mouseUsage = new QButtonGroup(frame);
//    mouseUsage->addButton(pointerPB, Chart::MouseUsageScroll);
//    mouseUsage->addButton(zoomInPB, Chart::MouseUsageZoomIn);
//    mouseUsage->addButton(zoomOutPB, Chart::MouseUsageZoomOut);
//    connect(mouseUsage, SIGNAL(buttonPressed(int)), SLOT(onMouseUsageChanged(int)));


//    // Update Period drop down list box
//    (new QLabel("Update Period (ms)", frame))->setGeometry(6, 200, 108, 16);
//    QComboBox *updatePeriod = new QComboBox(frame);
//    updatePeriod->setGeometry(6, 216, 108, 21);
//    updatePeriod->addItems(QStringList() << "1" << "250" << "500" << "750" << "1000" << "1250" << "1500"
//                           << "1750" << "2000");
//    connect(updatePeriod, SIGNAL(currentIndexChanged(QString)),
//            SLOT(onUpdatePeriodChanged(QString)));



    // The frame on the left side
    QFrame *frame_3 = new QFrame(frame);
    frame_3->setGeometry(0, 100, 150, 200);
    frame_3->setFrameShape(QFrame::NoFrame);

    QLabel *nowValue = new QLabel(frame_3);
    nowValue->setGeometry(0, 0, 150, 40);
    nowValue->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    nowValue->setText("瞬时测量值：");
    // Beta Value display
    QLabel *now_V = new QLabel(frame_3);
    now_V->setGeometry(0, 40, 150, 40);
    now_V->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:20px;}");
    now_V->setText("电压：");
    m_ValueB = new QLabel(frame_3);
    m_ValueB->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:20px;}");
    m_ValueB->setGeometry(60, 40, 150 - 60, 40);
    m_ValueB->setFrameShape(QFrame::NoFrame);
    // Gamma Value display
    QLabel *now_A = new QLabel(frame_3);
    now_A->setGeometry(0, 80, 150, 40);
    now_A->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:20px;}");
    now_A->setText("电流：");
    m_ValueC = new QLabel(frame_3);
    m_ValueC->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:20px;}");
    m_ValueC->setGeometry(60, 80, 150 - 60, 40);
    m_ValueC->setFrameShape(QFrame::NoFrame);
    // 瞬时功率
    QLabel *now_W = new QLabel(frame_3);
    now_W->setGeometry(0, 120, 150, 40);
    now_W->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:20px;}");
    now_W->setText("功率：");
    QLabel *now_W_Value = new QLabel(frame_3);
    now_W_Value->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:20px;}");
    now_W_Value->setGeometry(60, 120, 150 - 60, 40);
    now_W_Value->setFrameShape(QFrame::NoFrame);
    now_W_Value->setText(" - ");

    // The frame on the left side
    QFrame *frame_4 = new QFrame(frame);
    frame_4->setGeometry(0, 300, 150, 200);
    frame_4->setFrameShape(QFrame::NoFrame);
    QLabel *avgValue = new QLabel(frame_4);
    avgValue->setGeometry(0, 0, 150, 40);
    avgValue->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    avgValue->setText("平均测量值：");
    // Beta Value display
    QLabel *avg_V = new QLabel(frame_4);
    avg_V->setGeometry(0, 40, 150, 40);
    avg_V->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:20px;}");
    avg_V->setText("电压：");
    QLabel *buf1_QL = new QLabel(frame_4);
    buf1_QL->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:20px;}");
    buf1_QL->setGeometry(60, 40, 150 - 60, 40);
    buf1_QL->setFrameShape(QFrame::NoFrame);
    buf1_QL->setText(" - ");
    // Gamma Value display
    QLabel *avg_A = new QLabel(frame_4);
    avg_A->setGeometry(0, 80, 150, 40);
    avg_A->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:20px;}");
    avg_A->setText("电流：");
    QLabel *buf2_QL = new QLabel(frame_4);
    buf2_QL->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:20px;}");
    buf2_QL->setGeometry(60, 80, 150 - 60, 40);
    buf2_QL->setFrameShape(QFrame::NoFrame);
    buf2_QL->setText(" - ");
    // 瞬时功率
    QLabel *avg_W = new QLabel(frame_4);
    avg_W->setGeometry(0, 120, 150, 40);
    avg_W->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:20px;}");
    avg_W->setText("功率：");
    QLabel *buf3_QL = new QLabel(frame_4);
    buf3_QL->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:20px;}");
    buf3_QL->setGeometry(60, 120, 150 - 60, 40);
    buf3_QL->setFrameShape(QFrame::NoFrame);
    buf3_QL->setText(" - ");

//    // Run push button
//    QPushButton *sendUSB = new QPushButton("发送", frame);
//    sendUSB->setGeometry(4, 460, 112, 28);
//    sendUSB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
//    connect(sendUSB, SIGNAL(clicked(bool)), SLOT(onSendUSB()));
//    // Run push button
//    QPushButton *readUSB = new QPushButton("接收", frame);
//    readUSB->setGeometry(4, 490, 112, 28);
//    readUSB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
//    connect(readUSB, SIGNAL(clicked(bool)), SLOT(onReadUSB()));

    // The frame on the left side
    QFrame *frame_2 = new QFrame(this);
    frame_2->setGeometry(162, 4, 810, 820);
    frame_2->setFrameShape(QFrame::StyledPanel);

    // Chart Viewer
    m_ChartViewer = new QChartViewer(frame_2);
    m_ChartViewer->setGeometry(4, 8, 800, 500);
    connect(m_ChartViewer, SIGNAL(viewPortChanged()), SLOT(onViewPortChanged()));
    connect(m_ChartViewer, SIGNAL(mouseMovePlotArea(QMouseEvent*)),
        SLOT(onMouseMovePlotArea(QMouseEvent*)));

    // Horizontal scroll bar
    m_HScrollBar = new QScrollBar(Qt::Horizontal, frame_2);
    m_HScrollBar->setGeometry(4, 388, 800, 17);
    connect(m_HScrollBar, SIGNAL(valueChanged(int)), SLOT(onHScrollBarChanged(int)));

    // Chart Viewer 2
    m_ChartViewer_2 = new QChartViewer(frame_2);
    m_ChartViewer_2->setGeometry(4, 410, 800, 500);
    connect(m_ChartViewer_2, SIGNAL(viewPortChanged()), SLOT(onViewPortChanged_2()));
    connect(m_ChartViewer_2, SIGNAL(mouseMovePlotArea(QMouseEvent*)),
        SLOT(onMouseMovePlotArea_2(QMouseEvent*)));

    // Horizontal scroll bar
    m_HScrollBar_2 = new QScrollBar(Qt::Horizontal, frame_2);
    m_HScrollBar_2->setGeometry(4, 790, 800, 17);
    connect(m_HScrollBar_2, SIGNAL(valueChanged(int)), SLOT(onHScrollBarChanged_2(int)));

    onHScrollBarChanged(1); // 初始化显示表格
    onHScrollBarChanged_2(1);
//    onMouseMovePlotArea(nullptr);
//    m_ChartViewer_2->updateDisplay();

    // Initially set the mouse to drag to scroll mode.
    onMouseUsageChanged( Chart::MouseUsageScroll);      // 初始化表格鼠标事件的选中和缩放问题

    // Set m_nextDataTime to the current time. It is used by the real time random number
    // generator so it knows what timestamp should be used for the next data point.
    m_nextDataTime = QDateTime::currentDateTime();

    // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
    m_ChartViewer->setMouseWheelZoomRatio(1.1);
    m_ChartViewer_2->setMouseWheelZoomRatio(1.1);

    // Set up the data acquisition mechanism. In this demo, we just use a timer to get a
    // sample every 250ms.
    dataRateTimer = new QTimer(this);
    dataRateTimer->start(DataInterval);
    connect(dataRateTimer, SIGNAL(timeout()), SLOT(getData()));

    // Set up the data acquisition mechanism. In this demo, we just use a timer to get a
    // sample every 1ms.
    QTimer *dataRateTimer_2 = new QTimer(this);
    dataRateTimer_2->start(DataInterval);
    connect(dataRateTimer_2, SIGNAL(timeout()), SLOT(CreateData()));

    m_UsbReceiveThread = new USB_Receive_Thread(this, m_UsbHid, m_ComData);    // 新建线程
    connect(m_UsbReceiveThread,SIGNAL(get_USB_Data(unsigned char *, unsigned char )),this, SLOT(m_get_USB_Data(unsigned char *, unsigned char )));
    connect(m_UsbReceiveThread,SIGNAL(end_Thread()),this, SLOT(thread_finished()));
    // Set up the chart update timer
    m_ChartUpdateTimer = new QTimer(this);
    connect(m_ChartUpdateTimer, SIGNAL(timeout()), SLOT(updateChart()));
    updateChart();      // 初始化显示表格


    // Can start now
//    updatePeriod->setCurrentIndex(0);
//    runPB->click();


}

RealTime::~RealTime()
{
    qDebug() << "RealTime析构函数。";
    delete m_ChartViewer->getChart();
    delete m_ChartViewer_2->getChart();

    delete m_UsbHid;
//    if(m_ComData != nullptr)
//    {
//        delete m_ComData;
//        m_ComData = nullptr;
//    }
}

//
// Shift new data values into the real time data series
//
void RealTime::getData()
{
    // The current time
    QDateTime now = QDateTime::currentDateTime();
    int tmp = (int)(now.toTime_t() - m_ComData->lastTime.toTime_t());
    int nowValue = tmp * 1000 + now.time().msec() - m_ComData->lastTime.time().msec();
    if(nowValue < 250)
    {
        return;     // 100ms之内使用串口数据更新，否则将数据更新为0
    }
    // This is our formula for the random number generator
    do
    {
        // We need the currentTime in millisecond resolution
        double currentTime = Chart::chartTime2(m_ComData->lastTime.toTime_t())
                             + m_ComData->lastTime.time().msec() / 10 * 0.01;

//         qDebug() << "QTime测试：" << m_nextDataTime.time().msec();
//         qDebug() << "QTime测试：" << m_nextDataTime.time().msec() / 250 * 0.25;
//         qDebug() << "QTime测试：" << m_nextDataTime.time().msec() / 1000;
//         qDebug() << "QTime测试：" << m_nextDataTime.toTime_t();
//         qDebug() << "QTime测试：" << now.toTime_t();
//         qDebug() << "QTime测试：" << currentTime;
//         qDebug() << "QTime测试：m_nextDataTime = " << m_nextDataTime;
//         qDebug() << "QTime测试：now = " << now;

//        double dataA = 0;
        double dataB = 0;
        double dataC = 0;
//        // Get a data sample
//        double p = currentTime * 4;
//        double dataA = 20 + cos(p * 129241) * 10 + 1 / (cos(p) * cos(p) + 0.01);
//        double dataB = 150 + 100 * sin(p / 27.7) * sin(p / 10.1);
//        double dataC = 150 + 100 * cos(p / 6.7) * cos(p / 11.9);
//        if(m_ComData->d_currentIndex > 1)
//        {
//            dataB = m_ComData->d_dataSeriesV[m_ComData->d_currentIndex - 1];
//            dataC = m_ComData->d_dataSeriesA[m_ComData->d_currentIndex - 1];
//        }


        // After obtaining the new values, we need to update the data arrays.
        if (m_ComData->d_currentIndex < m_ComData->DataSize)
        {
            // Store the new values in the current index position, and increment the index.
            m_ComData->d_dataSeriesV[m_ComData->d_currentIndex] = dataB;
            m_ComData->d_dataSeriesA[m_ComData->d_currentIndex] = dataC;
            m_ComData->d_timeStamps[m_ComData->d_currentIndex] = currentTime;
            ++m_ComData->d_currentIndex;
        }
        else
        {
            // The data arrays are full. Shift the arrays and store the values at the end.
            ComData::shiftData_D(m_ComData->d_dataSeriesV, m_ComData->DataSize, dataB);
            ComData::shiftData_D(m_ComData->d_dataSeriesA, m_ComData->DataSize, dataC);
            ComData::shiftData_D(m_ComData->d_timeStamps, m_ComData->DataSize, currentTime);
        }

        m_ComData->lastTime = m_ComData->lastTime.addMSecs(DataInterval);
    }
    while (m_ComData->lastTime < now);

    //
    // We provide some visual feedback to the latest numbers generated, so you can see the
    // data being generated.
    //

    m_ValueB->setText(QString::number(m_ComData->d_dataSeriesV[m_ComData->d_currentIndex - 1], 'f', 2));
    m_ValueC->setText(QString::number(m_ComData->d_dataSeriesA[m_ComData->d_currentIndex - 1], 'f', 2));
}

////
//// User changes the chart update period
////
//void RealTime::onUpdatePeriodChanged(QString text)
//{
//    m_ChartUpdateTimer->start(text.toInt());
//}

//
// Update the chart. Instead of drawing the chart directly, we call updateViewPort, which
// will trigger a ViewPortChanged signal. We update the chart in the signal handler
// "drawChart". This can take advantage of the built-in rate control in QChartViewer to
// ensure a smooth user interface, even for extremely high update rate. (See the
// documentation on QChartViewer.setUpdateInterval).
//
void RealTime::updateChart()
{
    m_ChartViewer->updateViewPort(true, false);     // 这里可能会有问题
    m_ChartViewer_2->updateViewPort(true, false);
    onChartUpdateTimer(m_ChartViewer);
    onChartUpdateTimer(m_ChartViewer_2);
}

//
// Draw chart
//
void RealTime::drawChart()
{
//    return;
    // Create an XYChart object 600 x 270 pixels in size, with light grey (f4f4f4)
    // background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
    XYChart *c = new XYChart(800, 370, 0xf4f4f4, 0x000000, 1);
//    c->setDefaultFonts("Simsun","Simsun","Simsun");
    QColor bgColor = palette().color(backgroundRole()).rgb();
//    bgColor = 0x00f4f4;
    c->setRoundedFrame((bgColor.red() << 16) + (bgColor.green() << 8) + bgColor.blue());

    // Set the plotarea at (55, 62) and of size 520 x 175 pixels. Use white (ffffff)
    // background. Enable both horizontal and vertical grids by setting their colors to
    // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
    c->setPlotArea(55, 62, 720, 275, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
    c->setClipping();

    // Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
    // grey (dddddd) background, black (000000) border, and a glass like raised effect.
    c->addTitle("Chart View 1", "timesbi.ttf", 15)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());

    // Add a legend box at the top of the plot area with 9pts Arial Bold font. We set the
    // legend box to the same width as the plot area and use grid layout (as opposed to
    // flow or top/down layout). This distributes the 3 legend icons evenly on top of the
    // plot area.
    LegendBox *b = c->addLegend2(55, 33, 3, "arialbd.ttf", 9);
    b->setBackground(Chart::Transparent, Chart::Transparent);
    b->setWidth(520);

    // Configure the y-axis with a 10pts Arial Bold axis title
    c->yAxis()->setTitle("Voltage (V/mV)", "arialbd.ttf", 10);

    // Configure the x-axis to auto-scale with at least 75 pixels between major tick and
    // 15  pixels between minor ticks. This shows more minor grid lines on the chart.
    c->xAxis()->setTickDensity(75, 15);

    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Now we add the data to the chart.
    double lastTime = m_ComData->d_timeStamps[m_ComData->d_currentIndex - 1];
//    if (lastTime != Chart::NoValue)
//    {
        // Set up the x-axis to show the time range in the data buffer
        c->xAxis()->setDateScale(lastTime - DataInterval * m_ComData->d_currentIndex / 1000, lastTime);

        // Set the x-axis label format
        c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

        // Create a line layer to plot the lines
        LineLayer *layer = c->addLineLayer();

        // The x-coordinates are the timeStamps.
        layer->setXData(DoubleArray(m_ComData->d_timeStamps, m_ComData->d_currentIndex));

        // The 3 data series are used to draw 3 lines. Here we put the latest data values
        // as part of the data set name, so you can see them updated in the legend box.
//        layer->addDataSet(DoubleArray(m_dataSeriesA, m_currentIndex - 1), 0xff0000, "Alpha");
//        layer->addDataSet(DoubleArray(m_dataSeriesB, m_currentIndex - 1), 0x00cc00, "Beta");
//        layer->addDataSet(DoubleArray(m_dataSeriesC, m_currentIndex - 1), 0x0000ff, "Gamma");
        char buffer[1024];

        sprintf(buffer, "Hardware: <*bgColor=CCFFCC*> %.2f ", m_ComData->d_dataSeriesV[m_ComData->d_currentIndex - 1]);
        layer->addDataSet(DoubleArray(m_ComData->d_dataSeriesV, m_ComData->d_currentIndex), 0x00cc00, buffer);

        sprintf(buffer, "Services: <*bgColor=CCCCFF*> %.2f ", m_ComData->d_dataSeriesA[m_ComData->d_currentIndex - 1]);
        layer->addDataSet(DoubleArray(m_ComData->d_dataSeriesA, m_ComData->d_currentIndex), 0x0000ff, buffer);
//    }

    // Set the chart image to the WinChartViewer
    delete m_ChartViewer->getChart();
    m_ChartViewer->setChart(c);
//    delete c;
}

void RealTime::drawChart_2()
{
    XYChart *d = new XYChart(800, 370, 0xf4f4f4, 0x000000, 1);

    d->setPlotArea(55, 62, 720, 275, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
    d->setClipping();
    d->addTitle("Chart View 2", "timesbi.ttf", 15)->setBackground(0xdddddd, 0x000000, Chart::glassEffect());
    d->yAxis()->setTitle("Current (I/uA)", "arialbd.ttf", 10);
    d->xAxis()->setTickDensity(75, 15);
    d->xAxis()->setWidth(2);
    d->yAxis()->setWidth(2);

    // Now we add the data to the chart.
    double lastTime_2 = m_ComData->d_timeStamps[m_ComData->DataSize - 1];
    if (lastTime_2 != Chart::NoValue)
    {
        // Set up the x-axis to show the time range in the data buffer
        d->xAxis()->setDateScale(lastTime_2 - DataInterval * m_ComData->DataSize / 1000, lastTime_2);
        // Set the x-axis label format
        d->xAxis()->setLabelFormat("{value|hh:nn:ss}");
        // Create a line layer to plot the lines
        LineLayer *layer = d->addLineLayer();
        // The x-coordinates are the timeStamps.
        layer->setXData(DoubleArray(m_ComData->d_timeStamps, m_ComData->DataSize));
        // The 3 data series are used to draw 3 lines. Here we put the latest data values
        // as part of the data set name, so you can see them updated in the legend box.
        // The 3 data series are used to draw 3 lines.
//        layer->addDataSet(DoubleArray(m_dataSeriesA, m_ComData->DataSize), 0xff0000, "Alpha");
//        layer->addDataSet(DoubleArray(m_dataSeriesB, sampleSize), 0x00cc00, "Beta");
//        layer->addDataSet(DoubleArray(m_dataSeriesC, sampleSize), 0x0000ff, "Gamma");
    }
    // Include track line with legend. If the mouse is on the plot area, show the track
    // line with legend at the mouse position; otherwise, show them for the latest data
    // values (that is, at the rightmost position).
    trackLineLegend(d, m_ChartViewer_2->isMouseOnPlotArea() ? m_ChartViewer_2->getPlotAreaMouseX() :
        d->getPlotArea()->getRightX());

    // Set the chart image to the WinChartViewer
    delete m_ChartViewer_2->getChart();
    m_ChartViewer_2->setChart(d);

}
//
// Draw track cursor when mouse is moving over plotarea
//
void RealTime::onMouseMovePlotArea(QMouseEvent *)
{
    trackLineLabel((XYChart *)m_ChartViewer->getChart(), m_ChartViewer->getPlotAreaMouseX());
    m_ChartViewer->updateDisplay();
}
void RealTime::onMouseMovePlotArea_2(QMouseEvent *)
{
    trackLineLabel((XYChart *)m_ChartViewer_2->getChart(), m_ChartViewer_2->getPlotAreaMouseX());
    m_ChartViewer_2->updateDisplay();
}

//
// Draw the track line with legend
//
void RealTime::trackLineLegend(XYChart *c, int mouseX)
{
    // Clear the current dynamic layer and get the DrawArea object to draw on it.
    DrawArea *d = c->initDynamicLayer();

    // The plot area object
    PlotArea *plotArea = c->getPlotArea();

    // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
    double xValue = c->getNearestXValue(mouseX);
    int xCoor = c->getXCoor(xValue);

    // Draw a vertical track line at the x-position
    d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, d->dashLineColor(0x000000, 0x0101));

    // Container to hold the legend entries
    vector<string> legendEntries;

    // Iterate through all layers to build the legend array
    for (int i = 0; i < c->getLayerCount(); ++i) {
        Layer *layer = c->getLayerByZ(i);

        // The data array index of the x-value
        int xIndex = layer->getXIndexOf(xValue);

        // Iterate through all the data sets in the layer
        for (int j = 0; j < layer->getDataSetCount(); ++j) {
            DataSet *dataSet = layer->getDataSetByZ(j);

            // We are only interested in visible data sets with names
            const char *dataName = dataSet->getDataName();
            int color = dataSet->getDataColor();
            if (dataName && *dataName && (color != (int)Chart::Transparent)) {
                // Build the legend entry, consist of the legend icon, name and data value.
                double dataValue = dataSet->getValue(xIndex);
                ostringstream legendEntry;
                legendEntry << "<*block*>" << dataSet->getLegendIcon() << " " << dataName << ": " <<
                    ((dataValue == Chart::NoValue) ? "N/A" : c->formatValue(dataValue, "{value|P4}"))
                    << "<*/*>";
                legendEntries.push_back(legendEntry.str());

                // Draw a track dot for data points within the plot area
                int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());
                if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY())) {
                    d->circle(xCoor, yCoor, 4, 4, color, color);
                }
            }
        }
    }

    // Create the legend by joining the legend entries
    ostringstream legendText;
    legendText << "<*block,maxWidth=" << plotArea->getWidth() << "*><*block*><*font=arialbd.ttf*>["
        << c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss") << "]<*/*>";
    for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
        legendText << "        " << legendEntries[i];

    // Display the legend on the top of the plot area
    TTFText *t = d->text(legendText.str().c_str(), "arial.ttf", 8);
    t->draw(plotArea->getLeftX() + 5, plotArea->getTopY() - 3, 0x000000, Chart::BottomLeft);
    t->destroy();
}

//
// The Pointer, Zoom In or Zoom out button is pressed
//
void RealTime::onMouseUsageChanged(int mouseUsage)
{
    m_ChartViewer->setMouseUsage(mouseUsage);
    m_ChartViewer_2->setMouseUsage(mouseUsage);
}

//
// The Save button is pressed
//
void RealTime::onSave(bool)
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save", "chartdirector_demo",
        "PNG (*.png);;JPG (*.jpg);;GIF (*.gif);;BMP (*.bmp);;SVG (*.svg);;PDF (*.pdf)");

    if (!fileName.isEmpty())
    {
        // Save the chart
        BaseChart *c = m_ChartViewer_2->getChart();
        if (0 != c)
            c->makeChart(fileName.toUtf8().constData());
    }
}

//
// User clicks on the the horizontal scroll bar
//
void RealTime::onHScrollBarChanged(int value)
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
void RealTime::onHScrollBarChanged_2(int value)
{
    if (!m_ChartViewer_2->isInViewPortChangedEvent())
    {
        // Set the view port based on the scroll bar
        int scrollBarLen = m_HScrollBar_2->maximum() + m_HScrollBar_2->pageStep();
        m_ChartViewer_2->setViewPortLeft(value / (double)scrollBarLen);

        // Update the chart display without updating the image maps. (We can delay updating
        // the image map until scrolling is completed and the chart display is stable.)
        m_ChartViewer_2->updateViewPort(true, false);
    }
}

//
// View port changed event
//
void RealTime::onViewPortChanged()
{
    // In addition to updating the chart, we may also need to update other controls that
    // changes based on the view port.
    updateControls(m_ChartViewer, m_HScrollBar);

    // Update the chart if necessary
    if (m_ChartViewer->needUpdateChart())
        drawChart(m_ChartViewer, 0);
}
void RealTime::onViewPortChanged_2()
{
    // In addition to updating the chart, we may also need to update other controls that
    // changes based on the view port.
    updateControls(m_ChartViewer_2, m_HScrollBar_2);

    // Update the chart if necessary
    if (m_ChartViewer_2->needUpdateChart())
        drawChart(m_ChartViewer_2, 1);
}

//
// Update controls in the user interface when the view port changed
//
void RealTime::updateControls(QChartViewer *viewer, QScrollBar *bar)
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
void RealTime::drawChart(QChartViewer *viewer, int index)
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

    if (m_ComData->d_currentIndex > 0)
    {
        // Get the array indexes that corresponds to the visible start and end dates
        int startIndex = (int)floor(Chart::bSearch(DoubleArray(m_ComData->d_timeStamps, m_ComData->d_currentIndex), viewPortStartDate));
        int endIndex = (int)ceil(Chart::bSearch(DoubleArray(m_ComData->d_timeStamps, m_ComData->d_currentIndex), viewPortEndDate));
        int noOfPoints = endIndex - startIndex + 1;

        // Extract the visible data
        viewPortTimeStamps = DoubleArray(m_ComData->d_timeStamps+ startIndex, noOfPoints);
        viewPortDataSeriesB = DoubleArray(m_ComData->d_dataSeriesV + startIndex, noOfPoints);
        viewPortDataSeriesC = DoubleArray(m_ComData->d_dataSeriesA + startIndex, noOfPoints);
    }

    //
    // At this stage, we have extracted the visible data. We can use those data to plot the chart.
    //

    //================================================================================
    // Configure overall chart appearance.
    //================================================================================

    // Create an XYChart object of size 640 x 350 pixels
    XYChart *c = new XYChart(800, 390);

    // Set the plotarea at (55, 50) with width 80 pixels less than chart width, and height 80 pixels
    // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
    // as background. Set border to transparent and grid lines to white (ffffff).
    c->setPlotArea(55, 62, c->getWidth() - 85, c->getHeight() - 100, c->linearGradientColor(0, 50, 0,
        c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

    // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
    c->setClipping();

    // Add a title to the chart using 18pt Arial font
    if(index == 0)
        c->addTitle("Voltage with Zoom/Scroll and Track Line", "arial.ttf", 18);
    else if (index == 1)
        c->addTitle("Current with Zoom/Scroll and Track Line", "arial.ttf", 18);

    // Add a legend box at (55, 25) using horizontal layout. Use 10pt Arial Bold as font. Set the
    // background and border color to transparent and use line style legend key.
    LegendBox *b = c->addLegend(55, 25, false, "arialbd.ttf", 10);
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
        c->yAxis()->setTitle("Voltage (I/mV)", "arialbd.ttf", 12);
    else if (index == 1)
        c->yAxis()->setTitle("Current (I/uA)", "arialbd.ttf", 12);


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
//    layer->addDataSet(viewPortDataSeriesA, 0xff0000, "Alpha");
    if(index == 0)
        layer->addDataSet(viewPortDataSeriesB, 0x00cc00, "Voltage");
    else if(index == 1)
        layer->addDataSet(viewPortDataSeriesC, 0x0000ff, "Current");

    //================================================================================
    // Configure axis scale and labelling
    //================================================================================

    // Set the x-axis as a date/time axis with the scale according to the view port x range.
    if (m_ComData->d_currentIndex > 0)
        c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

    // For the automatic axis labels, set the minimum spacing to 75/30 pixels for the x/y axis.
    c->xAxis()->setTickDensity(75);
    c->yAxis()->setTickDensity(30);

    //
    // In this demo, the time range can be from many hours to a few seconds. We can need to define
    // the date/time format the various cases.
    //

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
    c->xAxis()->setMinTickInc(1);

    //================================================================================
    // Output the chart
    //================================================================================

    // We need to update the track line too. If the mouse is moving on the chart (eg. if
    // the user drags the mouse on the chart to scroll it), the track line will be updated
    // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
    if (!viewer->isInMouseMoveEvent())
    {
        trackLineLabel(c, (0 == viewer->getChart()) ? c->getPlotArea()->getRightX() :
            viewer->getPlotAreaMouseX());
    }

    // Set the chart image to the QChartViewer
    delete viewer->getChart();
    viewer->setChart(c);
}

//
// Draw the track line with data point labels
//
void RealTime::trackLineLabel(XYChart *c, int mouseX)
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
    ostringstream xlabel;
    xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss.ff")
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

                ostringstream label;
                label << "<*font,bgColor=" << hex << color << "*> "
                    << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
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

//
// Update the chart and the viewport periodically
//
void RealTime::onChartUpdateTimer(QChartViewer *viewer)
{
    if (m_ComData->d_currentIndex > 0)
    {
        //
        // As we added more data, we may need to update the full range of the viewport.
        //

        double startDate = m_ComData->d_timeStamps[0];
        double endDate = m_ComData->d_timeStamps[m_ComData->d_currentIndex - 1];

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

void RealTime::onConnectUSB()
{
    if(m_UsbHid->ConnectUSB())
    {
        connectUSB->setEnabled(false);
        disconnectUSB->setEnabled(true);
        qDebug() << "连接成功";
        m_UsbReceiveThread->isStop = false;
        m_UsbReceiveThread->start();   // 启动线程
        m_ChartUpdateTimer->start();    // 启动更新表格
        dataRateTimer->start(DataInterval); // 启动获取数据
    }
    else
    {
        qDebug() << "连接失败";
        QMessageBox::about(this, "提示", "连接失败！");
    }
}
void RealTime::onDisConnectUSB()
{
    m_UsbReceiveThread->isStop = true;
//    m_UsbReceiveThread->terminate();    // 关闭线程
//    m_UsbReceiveThread->wait();
    m_ChartUpdateTimer->stop();     // 关闭更新表格
    dataRateTimer->stop();      // 关闭获取数据
}

void RealTime::thread_finished()
{
    if(m_UsbHid->DisConnectUSB())
    {
        connectUSB->setEnabled(true);
        disconnectUSB->setEnabled(false);

        qDebug() << "关闭成功";
    }
    else
    {
        qDebug() << "关闭失败";
        QMessageBox::about(this, "提示", "关闭失败！");
    }
}
void RealTime::onSendUSB()
{
//    qDebug()<<"isrunning"<< m_UsbReceiveThread->isRunning();
//    qDebug()<<"isfinished"<< m_UsbReceiveThread->isFinished();
//    return;
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        return;
    }

    int res = -1;
    unsigned char sendBuf[32];
    unsigned char lengthSendBuf = 32;
    memset(sendBuf, 0, sizeof (sendBuf));

    sendBuf[0] = 0x12;
    sendBuf[1] = 0x34;
    sendBuf[2] = 0x56;
    sendBuf[3] = 0x78;
    sendBuf[4] = 0x90;

    res = m_UsbHid->SendUSB(sendBuf, lengthSendBuf);
    if(res >= 0)
    {
        qDebug() << "发送成功" << res;
    }
    else {
        qDebug() << "发送失败";
    }

}
void RealTime::onReadUSB()
{
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        return;
    }

    m_UsbHid->ReadUSB();
}

void RealTime:: CreateData()
{
//    return;
    if(m_UsbHid->dev_handle == nullptr)
    {
//        qDebug() << "USB设备未打开！";
        return;
    }
    // The current time
    QDateTime now = QDateTime::currentDateTime();
    // We need the currentTime in millisecond resolution
    double currentTime = Chart::chartTime2(now.toTime_t())
                         + now.time().msec() / 1 * 0.001;
    d_And_c dataB;
    d_And_c dataC;
    // Get a data sample
    double p = currentTime * 4;
//        double dataA = 20 + cos(p * 129241) * 10 + 1 / (cos(p) * cos(p) + 0.01);
    dataB.d = 150 + 100 * sin(p / 27.7) * sin(p / 10.1);
    dataC.d = 150 + 100 * cos(p / 6.7) * cos(p / 11.9);
//    if(ui->pushButton_5->text()==QString("关闭发送数据"))
//    {
        unsigned char sendP[32];
        memset(sendP, 0, sizeof (sendP));
        memcpy(sendP, m_ComData->headerC, m_ComData->headerLength); // 添加头码
        memcpy(sendP + 4, &dataB.c, 8);
        memcpy(sendP + 12, &dataC.c, 8);

//        serial.write(sendP, 20);    // 使用串口发送数据
        m_UsbHid->SendUSB(sendP, 32);   // 使用USB发送数据

        qDebug() << "单次发送的数据：dataB = " << dataB.d << ", dataC =" << dataC.d << " Time = " << QDateTime::currentDateTime();
//    }

}

void RealTime::m_get_USB_Data(unsigned char *buf, unsigned char len)
{

    unsigned char *getHeader = new unsigned char[4]{};
    memcpy(getHeader, buf, m_ComData->headerLength);
    if(0 == memcmp(getHeader, m_ComData->headerC, m_ComData->headerLength))
    {
      d_And_c dataB;
      d_And_c dataC;
      memcpy(&dataB, buf + 4, sizeof(double));
      memcpy(&dataC, buf + 12, sizeof(double));
      // 更新数据到表格数据
      // The current time
      QDateTime now = QDateTime::currentDateTime();
      m_ComData->lastTime = now;      // 更新接收时间
      // We need the currentTime in millisecond resolution
      double currentTime = Chart::chartTime2(now.toTime_t())
                           + now.time().msec() / 10 * 0.01;
      // After obtaining the new values, we need to update the data arrays.
      if (m_ComData->d_currentIndex < m_ComData->DataSize)
      {
          // Store the new values in the current index position, and increment the index.
          m_ComData->d_dataSeriesV[m_ComData->d_currentIndex] = dataB.d;
          m_ComData->d_dataSeriesA[m_ComData->d_currentIndex] = dataC.d;
          m_ComData->d_timeStamps[m_ComData->d_currentIndex] = currentTime;
          ++m_ComData->d_currentIndex;
      }
      else
      {
          // The data arrays are full. Shift the arrays and store the values at the end.
          ComData::shiftData_D(m_ComData->d_dataSeriesV, m_ComData->DataSize, dataB.d);
          ComData::shiftData_D(m_ComData->d_dataSeriesA, m_ComData->DataSize, dataC.d);
          ComData::shiftData_D(m_ComData->d_timeStamps, m_ComData->DataSize, currentTime);
      }
      qDebug() << "--单次接收的数据：dataB = " << dataB.d << ", dataC =" << dataC.d  << " Time = " << QDateTime::currentDateTime();
      m_ValueB->setText(QString::number(m_ComData->d_dataSeriesV[m_ComData->d_currentIndex - 1], 'f', 2));
      m_ValueC->setText(QString::number(m_ComData->d_dataSeriesA[m_ComData->d_currentIndex - 1], 'f', 2));
    }
    else {
        qDebug() << "接收数据的头码不对";
    }
}
