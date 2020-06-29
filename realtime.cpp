#include "realtime.h"
#include <math.h>
#include <vector>
#include <sstream>
#include <QDebug>
#include "mainwindow.h"
#include <QFontDatabase>
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>

using namespace std;

static const int DataInterval = 10;

RealTime::RealTime(QWidget *parent, ComData *comD, USB_HID *hid) : QWidget(parent)
{
    qDebug() << "RealTime构造函数。";
    m_ComData = comD;
    m_UsbHid = hid;
    m_About = nullptr;
    cntDisplay = 0;

    setGeometry(8, 10, 1280, 800);
//    setFixedSize(1090, 850);
////    setMinimumSize(200,200);
//    setWindowTitle("Simple Realtime Chart");

    // The frame on the left side
    QFrame *frame = new QFrame(this);
    frame->setGeometry(0, 0, 264, 777);
//    frame->setFixedSize(300, 910);
//    frame->setStyleSheet("background-color:white");
    frame->setFrameShape(QFrame::NoFrame);

    QString bnt_qss1 = "QPushButton{border-image: url(:/ButtonNormal.png);color:white; border:1px solid black;text-align:left; padding:2px; font-size:16px;}\
        QPushButton:hover{border-image: url(:/ButtonHover.png);color:#f0f0f0 ;}  \
        QPushButton:pressed{border-image: url(:/ButtonPressed.png);color:#e0e0e0 ;}\
        QPushButton:disabled{ color:#585858 ;}";
    QString bnt_qss2 = "QPushButton{border-image: url(:/ButtonNormal.png);color:white; border:1px solid black;text-align:left; padding:2px; font-size:22px;\
            border-top-left-radius:8px;         \
            border-top-right-radius:8px;        \
            border-bottom-left-radius:8px;     \
            border-bottom-right-radius:8px }   \
        QPushButton:hover{border-image: url(:/ButtonHover.png);color:#f0f0f0 ;}  \
        QPushButton:pressed{border-image: url(:/ButtonPressed.png);color:#e0e0e0 ;}\
        QPushButton:disabled{ color:#585858 ;}";
    QString frame_qss = "QFrame#FrameQss {border:1px solid black;\
            border-top-left-radius:4px;         \
            border-top-right-radius:4px;        \
            border-bottom-left-radius:4px;      \
            border-bottom-right-radius:4px}";
//    QString font_string = loadFontFamilyFromTTF(":/濑户字体简体.ttf");
//    int id=QFontDatabase::addApplicationFont(":/濑户字体简体.ttf");
    int fontId = QFontDatabase::addApplicationFont(":/ZhuoJianGanLanJianTi.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    int fontId_2 = QFontDatabase::addApplicationFont(":/SanariFontH001.ttf");
    QStringList fontFamilies_2 = QFontDatabase::applicationFontFamilies(fontId_2);
//    qDebug() << "fontId = " << fontId << " fontFamilies= " << fontFamilies.at(0);
//    qDebug() << "fontId_2 = " << fontId_2 << " fontFamilies_2= " << fontFamilies_2.at(0);
//    QFont    font ( fontFamilies.at(0));   //
    font.setFamily(fontFamilies.at(0));
//    QFont    font_2 ( fontFamilies_2.at(0));   //
    font_2.setFamily(fontFamilies_2.at(0));

    QFrame *frame_1 = new QFrame(frame);
    frame_1->setGeometry(2, 2, 260, 100);
//    frame_1->setFrameShape(QFrame::Panel);
    frame_1->setObjectName("FrameQss");
    frame_1->setStyleSheet(frame_qss);
    // Run push button
    connectUSB = new QPushButton(QIcon(":/play_red.png"), "启动采集", frame_1);
    connectUSB->setGeometry(2, 3, 130, 30);
    connectUSB->setStyleSheet(bnt_qss2);
    connectUSB->setFont(font);
    connect(connectUSB, &QAbstractButton::clicked, this, &RealTime::onConnectUSB);
    // Run push button
    disconnectUSB = new QPushButton(QIcon(":/pause.png"), "停止采集", frame_1);
    disconnectUSB->setGeometry(2, 35, 130, 30);
    disconnectUSB->setStyleSheet(bnt_qss2);
    disconnectUSB->setEnabled(false);
    disconnectUSB->setFont(font);
    connect(disconnectUSB, &QAbstractButton::clicked, this, &RealTime::onDisConnectUSB);
    // setting button
    QPushButton *settingBtn = new QPushButton(QIcon(":/setting_blue.png"), "采集设置", frame_1);
    settingBtn->setGeometry(2, 67, 130, 30);
    settingBtn->setStyleSheet(bnt_qss2);
    settingBtn->setFont(font);
    connect(settingBtn, &QAbstractButton::clicked, this, &RealTime::onSettingBtn);
//    QPushButton *bbb = new QPushButton(QIcon(":/play.png"),"", frame);
//    bbb->setGeometry(150, 8, 100, 100);
    QPixmap pixmap(":/logo.png");
    QLabel *label = new QLabel(frame_1);
    label->setGeometry(150, 3, 100, 95);
    label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

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
    QString fontName = loadFontFamilyFromTTF(":/DSDigital.ttf");    // digifaw ":/DSDigital.ttf" "C:/Windows/Fonts/Terminal.fon"
    QFont    font_VAW ( fontName,  10,   100);     // "Microsoft YaHei"
    QFrame *frame_3 = new QFrame(frame);
    frame_3->setGeometry(2, 105, 260, 280);
//    frame_3->setFrameShape(QFrame::Panel);
    frame_3->setObjectName("FrameQss");
    frame_3->setStyleSheet(frame_qss);
    QLabel *realTimeValue = new QLabel(frame_3);
    realTimeValue->setGeometry(0, 2, 250, 30);
    realTimeValue->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    realTimeValue->setFont(font);
    realTimeValue->setText("实时采集值：");
    // 显示瞬时电压/电流/功率
    m_ValueB = new QLabel(frame_3);
    m_ValueB->setStyleSheet("QLabel { text-align:center; padding:10px; font-size:60px; color:#00cc00; }");      // background-color:white;
    m_ValueB->setGeometry(-25, 80, 250, 70);
    m_ValueB->setFont(font_VAW);
    m_ValueB->setAlignment(Qt::AlignHCenter);
    m_ValueB->setFrameShape(QFrame::NoFrame);
    m_ValueB->setText(QString::number(0, 'f', 3));  // 初始化显示0
    QLabel *unitV = new QLabel(frame_3);
    unitV->setGeometry(200, 80 + 10, 64, 70);
    unitV->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:28px; color:#00cc00; }");
    unitV->setText("V");

    m_ValueC = new QLabel(frame_3);
    m_ValueC->setStyleSheet("QLabel { text-align:center; padding:10px; font-size:60px; color:blue;}");
    m_ValueC->setGeometry(-25, 20, 250, 70);
    m_ValueC->setFont(font_VAW);
    m_ValueC->setAlignment(Qt::AlignHCenter);
    m_ValueC->setFrameShape(QFrame::NoFrame);
    m_ValueC->setText(QString::number(0, 'f', 3));  // 初始化显示0
    m_unitA = new QLabel(frame_3);
    m_unitA->setGeometry(200, 20 + 10, 64, 70);
    m_unitA->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:28px; color:blue; }");
    m_unitA->setText("mA");

    m_Power = new QLabel(frame_3);
    m_Power->setStyleSheet("QLabel { text-align:left; padding:10px; font-size:60px; color:black;}");
    m_Power->setGeometry(-25, 140, 250, 70);
    m_Power->setFont(font_VAW);
    m_Power->setAlignment(Qt::AlignHCenter);
    m_Power->setFrameShape(QFrame::NoFrame);
    m_Power->setText(QString::number(0, 'f', 2));  // 初始化显示0
    QLabel *unitW = new QLabel(frame_3);
    unitW->setGeometry(200, 140 + 10, 68, 70);
    unitW->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:28px; color:black; }");
    unitW->setText("W");

    m_Energy = new QLabel(frame_3);
    m_Energy->setStyleSheet("QLabel { text-align:left; padding:10px; font-size:60px; color:black; }");
    m_Energy->setGeometry(-25, 200, 250, 70);
    m_Energy->setFont(font_VAW);
    m_Energy->setAlignment(Qt::AlignHCenter);
    m_Energy->setFrameShape(QFrame::NoFrame);
    m_Energy->setText(QString::number(0, 'f', 2));  // 初始化显示0
    QLabel *unitE = new QLabel(frame_3);
    unitE->setGeometry(200, 200 + 10, 68, 70);
    unitE->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:24px; color:black; }");
    unitE->setText("mAh");

    // The frame on the left side
    QFrame *frame_4 = new QFrame(frame);
    frame_4->setGeometry(2, 390, 260, 220);
//    frame_4->setFrameShape(QFrame::Panel);
    frame_4->setObjectName("FrameQss");
    frame_4->setStyleSheet(frame_qss);
    QLabel *avgValue = new QLabel(frame_4);
    avgValue->setGeometry(0, 2, 250, 30);
    avgValue->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    avgValue->setFont(font);
    avgValue->setText("平均采集值：");
    // avarge voltage
    buf1_QL = new QLabel(frame_4);
    buf1_QL->setStyleSheet("QLabel { text-align:center; padding:10px; font-size:64px; color:#00cc00;}");
    buf1_QL->setGeometry(-25, 20 + 60, 250, 80);
    buf1_QL->setFont(font_VAW);
    buf1_QL->setAlignment(Qt::AlignHCenter);
    buf1_QL->setFrameShape(QFrame::NoFrame);
    buf1_QL->setText("-");  // 初始化显示-
//    buf1_QL->setText(QString::number(0, 'f', 3));  // 初始化显示0
    QLabel *avg_V = new QLabel(frame_4);
    avg_V->setGeometry(200, 20 + 70, 64, 70);
    avg_V->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:28px; color:#00cc00; }");
    avg_V->setText("V");
    // avarge current
    buf2_QL = new QLabel(frame_4);
    buf2_QL->setStyleSheet("QLabel { text-align:center; padding:10px; font-size:64px; color:blue;}");
    buf2_QL->setGeometry(-25, 20, 250, 80);
    buf2_QL->setFont(font_VAW);
    buf2_QL->setAlignment(Qt::AlignHCenter);
    buf2_QL->setFrameShape(QFrame::NoFrame);
    buf2_QL->setText("-");  // 初始化显示-
//    buf2_QL->setText(QString::number(0, 'f', 3));  // 初始化显示0
    m_averageA = new QLabel(frame_4);
    m_averageA->setGeometry(200, 20 + 10, 64, 70);
    m_averageA->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:28px; color:blue; }");
    m_averageA->setText("mA");
    // avarge power
    buf3_QL = new QLabel(frame_4);
    buf3_QL->setStyleSheet("QLabel { text-align:left; padding:10px; font-size:64px; color:black; }");
    buf3_QL->setGeometry(-25, 140, 250, 80);
    buf3_QL->setFont(font_VAW);
    buf3_QL->setAlignment(Qt::AlignHCenter);
    buf3_QL->setFrameShape(QFrame::NoFrame);
    buf3_QL->setText("-");  // 初始化显示-
//    buf3_QL->setText(QString::number(0, 'f', 2));  // 初始化显示0
    QLabel *avg_W = new QLabel(frame_4);
    avg_W->setGeometry(200, 140 + 10, 64, 70);
    avg_W->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:28px; color:black; }");
    avg_W->setText("W");

    QFrame *frame_6 = new QFrame(frame);
    frame_6->setGeometry(2, 615, 260, 160);
//    frame_6->setFrameShape(QFrame::Panel);
    frame_6->setObjectName("FrameQss");
    frame_6->setStyleSheet(frame_qss);
    QLabel *batteryInfo = new QLabel(frame_6);
    batteryInfo->setGeometry(0, 2, 250, 30);
    batteryInfo->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    batteryInfo->setFont(font);
    batteryInfo->setText("电池信息：");
    QLabel *totalCapacity = new QLabel(frame_6);
    totalCapacity->setGeometry(0, 30, 250, 40);
    totalCapacity->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    totalCapacity->setFont(font);
    totalCapacity->setText("总容量：");
    QLabel *totalCapacityUnit = new QLabel(frame_6);
    totalCapacityUnit->setGeometry(200, 30, 250, 40);
    totalCapacityUnit->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:18px;}");
    totalCapacityUnit->setFont(font);
    totalCapacityUnit->setText("mAh");
    bTotalCap = new QLabel(frame_6);
    bTotalCap->setGeometry(110, 30 + 8, 80, 40);
//    bTotalCap->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:2px; font-size:24px;}");
    bTotalCap->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:30px; color:black; }");
    bTotalCap->setFont(font_VAW);
    bTotalCap->setAlignment(Qt::AlignLeft);
    bTotalCap->setFrameShape(QFrame::NoFrame);
    bTotalCap->setText(QString::number(m_ComData->SettingBatteryCapacity));
    QLabel *remainCapacity = new QLabel(frame_6);
    remainCapacity->setGeometry(0, 60, 250, 40);
    remainCapacity->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    remainCapacity->setFont(font);
    remainCapacity->setText("剩余容量：");
    bRemainCap = new QLabel(frame_6);
    bRemainCap->setGeometry(140, 60 + 8, 100, 40);
//    bRemainCap->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:2px; font-size:24px;}");
    bRemainCap->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:30px; color:black; }");
    bRemainCap->setFont(font_VAW);
    bRemainCap->setAlignment(Qt::AlignLeft);
    bRemainCap->setFrameShape(QFrame::NoFrame);
    bRemainCap->setText("100%");
    QLabel *workingTime = new QLabel(frame_6);
    workingTime->setGeometry(0, 90, 260, 40);
    workingTime->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    workingTime->setFont(font);
    workingTime->setText("运行时间：   时  分");
    bRunningTimeHour = new QLabel(frame_6);
    bRunningTimeHour->setGeometry(120, 90 + 8, 80, 40);
    bRunningTimeHour->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:30px; color:black; }");
    bRunningTimeHour->setFont(font_VAW);
    bRunningTimeHour->setAlignment(Qt::AlignLeft);
    bRunningTimeHour->setFrameShape(QFrame::NoFrame);
    bRunningTimeHour->setText(" -");
    bRunningTimeMinute = new QLabel(frame_6);
    bRunningTimeMinute->setGeometry(197, 90 + 8, 40, 40);
    bRunningTimeMinute->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:30px; color:black; }");
    bRunningTimeMinute->setFont(font_VAW);
    bRunningTimeMinute->setAlignment(Qt::AlignLeft);
    bRunningTimeMinute->setFrameShape(QFrame::NoFrame);
    bRunningTimeMinute->setText(" -");
    QLabel *remainTime = new QLabel(frame_6);
    remainTime->setGeometry(0, 120, 260, 40);
    remainTime->setStyleSheet("QLabel { text-align:left; padding:1px; font-size:24px;}");
    remainTime->setFont(font);
    remainTime->setText("剩余时间：   时  分");
    bRemainTimeHour = new QLabel(frame_6);
    bRemainTimeHour->setGeometry(120, 120 + 8, 80, 40);
    bRemainTimeHour->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:30px; color:black; }");
    bRemainTimeHour->setFont(font_VAW);
    bRemainTimeHour->setAlignment(Qt::AlignLeft);
    bRemainTimeHour->setFrameShape(QFrame::NoFrame);
    bRemainTimeHour->setText(" -");
    bRemainTimeMinute = new QLabel(frame_6);
    bRemainTimeMinute->setGeometry(197, 120 + 8, 40, 40);
    bRemainTimeMinute->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:30px; color:black; }");
    bRemainTimeMinute->setFont(font_VAW);
    bRemainTimeMinute->setAlignment(Qt::AlignLeft);
    bRemainTimeMinute->setFrameShape(QFrame::NoFrame);
    bRemainTimeMinute->setText(" -");

    QFrame *frame_5 = new QFrame(frame);
    frame_5->setGeometry(0, 560, 250, 280);
    frame_5->setFrameShape(QFrame::NoFrame);
    frame_5->setVisible(false);     // 先屏蔽这个显示区
    QLabel *USBConfig = new QLabel(frame_5);
    USBConfig->setGeometry(0, 0, 250, 40);
    USBConfig->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:2px; font-size:30px;}");
    USBConfig->setText("设备信息：");
    QLabel *usb_M = new QLabel(frame_5);
    usb_M->setGeometry(0, 40, 250, 40);
    usb_M->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:2px; font-size:19px;}");
    usb_M->setText("厂商:");    // Manufactured
    usb_str1 = new QLabel(frame_5);
    usb_str1->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:0px; font-size:16px;}");
    usb_str1->setGeometry(50, 40, 250 - 50, 40);
    usb_str1->setFrameShape(QFrame::NoFrame);
    usb_str1->setText("-");
    QLabel *usb_P = new QLabel(frame_5);
    usb_P->setGeometry(0, 80, 250, 40);
    usb_P->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:2px; font-size:19px;}");
    usb_P->setText("产品:");     // Product
    usb_str2 = new QLabel(frame_5);
    usb_str2->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:0px; font-size:16px;}");
    usb_str2->setGeometry(50, 80, 250 - 50, 40);
    usb_str2->setFrameShape(QFrame::NoFrame);
    usb_str2->setText("-");
    QLabel *usb_S = new QLabel(frame_5);
    usb_S->setGeometry(0, 120, 250, 40);
    usb_S->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:2px; font-size:19px;}");
    usb_S->setText("序列号:");    // SerialNumber
    usb_str3 = new QLabel(frame_5);
    usb_str3->setStyleSheet("QLabel {font-family:arial; text-align:left; padding:0px; font-size:16px;}");
    usb_str3->setGeometry(70, 120, 250 - 70, 40);
    usb_str3->setFrameShape(QFrame::NoFrame);
    usb_str3->setText("-");
    // 显示温度
    m_Temp = new QLabel(frame_5);
    m_Temp->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:20px;}");
    m_Temp->setGeometry(0, 100, 250, 40);
    m_Temp->setFrameShape(QFrame::NoFrame);
    m_Temp->setFont(font);
    m_Temp->setText("设备内部温度:-");

    m_Tips = new QLabel(frame_5);
    m_Tips->setStyleSheet("QLabel { text-align:left; padding:2px; color:red; font-size:18px;}");
    m_Tips->setGeometry(0, 140, 250, 20);
    m_Tips->setFrameShape(QFrame::NoFrame);
//    m_Tips->setText("电流过大:");
    m_Error = new QLabel(frame_5);
    m_Error->setStyleSheet("QLabel { text-align:left; padding:2px; color:red; font-size:18px;}");
    m_Error->setGeometry(0, 160, 250, 20);
    m_Error->setFrameShape(QFrame::NoFrame);
//    m_Error->setText("错误代码:");

    QDateTime lastTime = QDateTime::currentDateTime();
    QLabel *linkUs = new QLabel(frame_5);
    linkUs->setGeometry(40, 190, 250, 40);
    linkUs->setStyleSheet("QLabel {font-family:castellar; text-align:left; padding:2px; font-size:20px;}");
    linkUs->setText("<a href=\"http://www.baidu.com\">© " + QString::number(lastTime.date().year()) + " 关于我们");
    connect(linkUs,SIGNAL(linkActivated (QString)),this,SLOT(linkUs(QString)));   // const QString&

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
    QFrame *frame_2 = new QFrame(this);     // 波形显示界面
    frame_2->setGeometry(0, 0, 1000, 780);
//    frame_2->setFrameShape(QFrame::StyledPanel);
    QFrame *frame_2_ext = new QFrame(this);     // 数据显示界面
    frame_2_ext->setGeometry(0, 0, 1000, 780);
//    frame_2_ext->setBackgroundRole(QPalette::shadow());
//    frame_2_ext->setFrameShape(QFrame::StyledPanel);
    frame_2_updata = new QFrame(this);     // 更新程序界面
    frame_2_updata->setGeometry(0, 0, 1000, 780);
//    frame_2_updata->setBackgroundRole(QPalette::shadow());
//    frame_2_updata->setFrameShape(QFrame::StyledPanel);

    tabWidget = new QTabWidget(this);
    tabWidget->setGeometry(265, 0, 1000, 780);
    tabWidget->addTab(frame_2, "最新数据");
    tabWidget->addTab(frame_2_ext, "历史数据");
//    tabWidget->addTab(frame_2_updata, "设置");
    QString tabBarStyle = "QTabWidget::tab-bar{ alignment:left;}\
            QTabBar::tab{border-color: #805533; background-color: rgb(96, 96, 96); /*灰色*/ color:white; width:150px; min-height:10px; border: 2px solid #FFFFFF; padding:5px;font-size:18px;}\
            QTabBar::tab:selected{border-image: url(:/ButtonPressed.png); background-color: white; color:white; font-weight:bold; border: 2px solid #0066CC; font-size:22px;}\
            QTabBar::tab:!selected { margin-top: 5px;}";
    tabWidget->setStyleSheet(tabBarStyle);
    tabWidget->setFont(font);
//    tabWidget->removeTab(tabWidget->count()-1);
    frame_2_updata->setVisible(false);
//    connect(tabWidget, SIGNAL(tabBarClicked(2)), this, SLOT(&RealTime::onSettingBtn));
//    QHBoxLayout *layout = new QHBoxLayout(this);
//    layout->addWidget(tabWidget);

/*  控件自动缩放
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(frame);
    leftLayout->addStretch(); //在close按键下方添加一个弹簧填空
    leftLayout->setMargin(0);

    QVBoxLayout *rightLayout = new QVBoxLayout;
//        rightLayout->addLayout(topLeftLayout);
    rightLayout->addWidget(tabWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);      */

    // Chart Viewer
    m_ChartViewer = new QChartViewer(frame_2);
    m_ChartViewer->setGeometry(4, 445 - 96, 1345 - 360 + 10, 535);
    connect(m_ChartViewer, SIGNAL(viewPortChanged()), SLOT(onViewPortChanged()));
    connect(m_ChartViewer, SIGNAL(mouseMovePlotArea(QMouseEvent*)),
        SLOT(onMouseMovePlotArea(QMouseEvent*)));
    connect(m_ChartViewer, SIGNAL(clicked(QMouseEvent*)),
        SLOT(onMouseClick(QMouseEvent*)));

    // Horizontal scroll bar
    m_HScrollBar = new QScrollBar(Qt::Horizontal, frame_2);
    m_HScrollBar->setGeometry(4, 855 - 132, 1345 - 360 + 10, 17);
    connect(m_HScrollBar, SIGNAL(valueChanged(int)), SLOT(onHScrollBarChanged(int)));

    // Chart Viewer 2
    m_ChartViewer_2 = new QChartViewer(frame_2);
    m_ChartViewer_2->setGeometry(4, -16, 1345 - 360 + 10, 535);
    connect(m_ChartViewer_2, SIGNAL(viewPortChanged()), SLOT(onViewPortChanged_2()));
    connect(m_ChartViewer_2, SIGNAL(mouseMovePlotArea(QMouseEvent*)),
        SLOT(onMouseMovePlotArea_2(QMouseEvent*)));
    connect(m_ChartViewer_2, SIGNAL(clicked(QMouseEvent*)),
        SLOT(onMouseClick_2(QMouseEvent*)));

    // Horizontal scroll bar
    m_HScrollBar_2 = new QScrollBar(Qt::Horizontal, frame_2);
    m_HScrollBar_2->setGeometry(4, 423 - 66, 1345 - 360 + 10, 17);
    connect(m_HScrollBar_2, SIGNAL(valueChanged(int)), SLOT(onHScrollBarChanged_2(int)));

    /* 控件自动缩放
    QVBoxLayout *frameLayout = new QVBoxLayout(frame_2);
    frameLayout->addWidget(m_ChartViewer_2);
    frameLayout->addWidget(m_HScrollBar_2);
    frameLayout->addWidget(m_ChartViewer);
    frameLayout->addWidget(m_HScrollBar);
    frameLayout->setMargin(0);  */
//    qDebug() << "RealTime 完成控件初始化";
//    onHScrollBarChanged(1); // 初始化显示表格        // 这两行不要，加上这两行会出现程序无法正常开启
//    onHScrollBarChanged_2(1);
//    onMouseMovePlotArea(nullptr);
//    m_ChartViewer_2->updateDisplay();
//    qDebug() << "RealTime 完成ScrollBar初始化";
    // Initially set the mouse to drag to scroll mode.
    onMouseUsageChanged( Chart::MouseUsageScroll);      // 初始化表格鼠标事件的选中和缩放问题
//    qDebug() << "RealTime 完成鼠标事件初始化";
    // Set m_nextDataTime to the current time. It is used by the real time random number
    // generator so it knows what timestamp should be used for the next data point.
    m_nextDataTime = QDateTime::currentDateTime();
//    qDebug() << "RealTime 完成QDateTime初始化";
    // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
    m_ChartViewer->setMouseWheelZoomRatio(1.1);
    m_ChartViewer_2->setMouseWheelZoomRatio(1.1);

    // Set up the chart update timer
    m_ChartUpdateTimer = new QTimer(this);
    connect(m_ChartUpdateTimer, SIGNAL(timeout()), SLOT(updateChart()));
    qDebug() << "RealTime 到了完成updateChart之前";
//    m_ChartViewer->setChart(new XYChart(1345 - 360 + 10, 425 - 40));
//    m_ChartViewer_2->setChart(new XYChart(1345 - 360 + 10, 425 - 40));
//    drawChart(m_ChartViewer, 0);
//    drawChart(m_ChartViewer_2, 1);
    updateChart();      // 初始化显示表格
    qDebug() << "RealTime 到了完成updateChart";

    m_UsbReceiveThread = new USB_Receive_Thread(this, m_UsbHid, m_ComData);    // 新建线程
//    m_UsbReceiveThread->setPriority(QThread::IdlePriority);
//    connect(m_UsbReceiveThread,SIGNAL(get_Vol_Cur_Now(qint64, double, double)),this, SLOT(writeSQL(qint64, double, double))); // 写数据库
    connect(m_UsbReceiveThread,SIGNAL(get_USB_Data(QDateTime, double, unsigned char, unsigned char)),this, SLOT(m_get_USB_Data(QDateTime, double, unsigned char, unsigned char)));
    connect(m_UsbReceiveThread,SIGNAL(get_Version_Length(unsigned long long, unsigned long long)),this, SLOT(m_get_Version_Length(unsigned long long, unsigned long long)));
    connect(m_UsbReceiveThread,SIGNAL(end_Thread()),this, SLOT(thread_receive_finished()));

    m_UsbSendThread = new USB_Send_Thread(this, m_UsbHid, m_ComData);    // 新建线程
//    m_UsbSendThread->setPriority(QThread::IdlePriority);
    connect(m_UsbSendThread,SIGNAL(end_Thread()),this, SLOT(thread_send_finished()));
    connect(m_UsbSendThread,SIGNAL(sendProgressBar(unsigned long)),this, SLOT(updataProgressBar(unsigned long)));
    connect(m_UsbSendThread,SIGNAL(usbSuccess()),this, SLOT(upadataSuccess()));
    connect(m_UsbSendThread,SIGNAL(usbFail()),this, SLOT(updataFail()));
    connect(m_UsbSendThread,SIGNAL(usbTimeOut()),this, SLOT(upadtaTimeOut()));
    connect(m_UsbReceiveThread,SIGNAL(setAckOrNak(int)),m_UsbSendThread, SLOT(setAckState(int)));

    m_SqliteThread = new sqlite_thread(this, m_UsbHid, m_ComData);    // 新建线程
    connect(m_SqliteThread,SIGNAL(end_Thread()),this, SLOT(thread_sqlite_finished()));
    connect(m_SqliteThread,SIGNAL(show_Energy()),this, SLOT(slotShowEnergy()));
    connect(m_SqliteThread,SIGNAL(show_Average()),this, SLOT(showAverage()));
    connect(m_SqliteThread,SIGNAL(show_time(qint64, qint64)),this, SLOT(slotShowTime(qint64, qint64)));
    connect(this,SIGNAL(CreateSqilite()),m_SqliteThread, SLOT(CreateSqlite_T()));
    connect(m_SqliteThread,SIGNAL(emitQBoxTip(QString)),this, SLOT(slotQBoxTip(QString)));
    connect(m_UsbReceiveThread,SIGNAL(get_Vol_Cur_Now(qint64, double, double)),m_SqliteThread, SLOT(writeSqliteData(qint64, double, double)));  // 写数据库
//    m_SqliteThread->start();


    QLabel *volTitle = new QLabel(frame_2);
    volTitle->setGeometry(840 - 420, 455 - 4 - 66 - 5, 300, 30);
    volTitle->setStyleSheet("QLabel {text-align:left; padding:0px; font-size:28px; background-color:white;}");
    volTitle->setText("电压测量波形图");
    volTitle->setFont(font_2);
    QLabel *curTitle = new QLabel(frame_2);
    curTitle->setGeometry(840 - 420, 18 - 4, 300, 30);
    curTitle->setStyleSheet("QLabel {text-align:left; padding:0px; font-size:28px; background-color:white;}");
    curTitle->setText("电流测量波形图");
    curTitle->setFont(font_2);
    /*
    QLabel *volShow = new QLabel(frame_2);
    volShow->setGeometry(360 - 262, 485 - 4, 60, 20);
    volShow->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:20px; color:#00cc00; background-color:white;}");
    volShow->setText("电压：");
    volShow->setFont(font);
    QLabel *curShow = new QLabel(frame_2);
    curShow->setGeometry(360 - 262, 48 - 4, 60, 20);
    curShow->setStyleSheet("QLabel {font-family:elephant; text-align:left; padding:0px; font-size:20px; color:blue; background-color:white;}");
    curShow->setText("电流：");
    curShow->setFont(font); */

//    // 初始化显示
//    dataView->setModel(dataModel);
//    dataView->setColumnWidth(0,70);dataView->setColumnWidth(1,230);dataView->setColumnWidth(2,160);dataView->setColumnWidth(3,160);dataView->setColumnWidth(4,160);
//    dataView->setUpdatesEnabled(true);  //恢复界面刷新
     historyView = new HistoryView(frame_2_ext);    // 历史数据表格界面
     historyView->setGeometry(0, 0, 1000, 780);
//    historyDetail = new HistoryDetail(frame_2_ext);
//    historyDetail->setGeometry(0, 0, 1000, 780);
//    connect(this,SIGNAL(SignalsTest()),historyDetail, SLOT(ReceiveTest()));
    emit SignalsTest();
    historyFile = new QPushButton(frame_2_ext);
    historyFile->setStyleSheet("QPushButton {font-family:arial; text-align:left; padding:5px; font-size:18px; border:1px solid #000000;}");
    historyFile->setGeometry(4, 5, 1180-4-360, 30);
//    historyFile->setFrameShape(QFrame::NoFrame);
    historyFile->setText("");
    connect(historyFile, &QAbstractButton::clicked, this, &RealTime::HistoryOpen);
    historyOpen = new QPushButton( " 加载文件", frame_2_ext);
    historyOpen->setGeometry(1182-360, 5, 130, 30);
    historyOpen->setStyleSheet(bnt_qss2);
    historyOpen->setFont(font);
    connect(historyOpen, &QAbstractButton::clicked, this, &RealTime::HistoryOpen);

    QFrame *frame_setting = new QFrame(frame_2_updata);     // 波形显示界面
    frame_setting->setGeometry(100, 0, 1000 - 100, 780);
    frame_setting->setFrameShape(QFrame::NoFrame);

    QFrame *firstLine1 = new QFrame(frame_setting);
    firstLine1->setGeometry(QRect(5, 20, 340, 2));
    firstLine1->setFrameShape(QFrame::HLine);
    firstLine1->setFrameShadow(QFrame::Sunken);
    firstLine1->raise();
    QLabel *firstTitle = new QLabel(frame_setting);
    firstTitle->setGeometry(360, 20-20, 100, 40);
    firstTitle->setStyleSheet("QLabel { text-align:left; font-size:20px;}");
    firstTitle->setFont(font);
    firstTitle->setText("程序升级");    // SerialNumber
    QFrame *firstLine2 = new QFrame(frame_setting);
    firstLine2->setGeometry(QRect(450, 20, 350, 2));
    firstLine2->setFrameShape(QFrame::HLine);
    firstLine2->setFrameShadow(QFrame::Sunken);
    firstLine2->raise();

    QFrame *secondLine1 = new QFrame(frame_setting);
    secondLine1->setGeometry(QRect(5, 200, 340, 2));
    secondLine1->setFrameShape(QFrame::HLine);
    secondLine1->setFrameShadow(QFrame::Sunken);
    secondLine1->raise();
    QLabel *secondTitle = new QLabel(frame_setting);
    secondTitle->setGeometry(360, 200-20, 100, 40);
    secondTitle->setStyleSheet("QLabel { text-align:left; font-size:20px;}");
    secondTitle->setFont(font);
//    secondTitle->setAttribute(Qt::WA_TranslucentBackground);
    secondTitle->setText("参数设定");    // SerialNumber
    QFrame *secondLine2 = new QFrame(frame_setting);
    secondLine2->setGeometry(QRect(450, 200, 350, 2));
    secondLine2->setFrameShape(QFrame::HLine);
    secondLine2->setFrameShadow(QFrame::Sunken);
    secondLine2->raise();

    QFrame *ThirdLine1 = new QFrame(frame_setting);
    ThirdLine1->setGeometry(QRect(5, 650, 340, 2));
    ThirdLine1->setFrameShape(QFrame::HLine);
    ThirdLine1->setFrameShadow(QFrame::Sunken);
    ThirdLine1->raise();
    QLabel *ThirdTitle = new QLabel(frame_setting);
    ThirdTitle->setGeometry(360, 650-20, 100, 40);
    ThirdTitle->setStyleSheet("QLabel { text-align:left; font-size:20px;}");
    ThirdTitle->setFont(font);
//    ThirdTitle->setAttribute(Qt::WA_TranslucentBackground);
    ThirdTitle->setText("关于我们");    // SerialNumber
    QFrame *ThirdLine2 = new QFrame(frame_setting);
    ThirdLine2->setGeometry(QRect(450, 650, 350, 2));
    ThirdLine2->setFrameShape(QFrame::HLine);
    ThirdLine2->setFrameShadow(QFrame::Sunken);
    ThirdLine2->raise();

    QLabel *appInfo = new QLabel(frame_setting);
    appInfo->setGeometry(10, 30, 130, 50);
    appInfo->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    appInfo->setFont(font_2);
    appInfo->setText("程序版本:");    // SerialNumber
    appVersion = new QLabel(frame_setting);
    appVersion->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:24px;}");
    appVersion->setFont(font_2);
    appVersion->setGeometry(140, 30, 200, 50);
    appVersion->setFrameShape(QFrame::NoFrame);
    appVersion->setText("-");
    QLabel *appInfo_2 = new QLabel(frame_setting);
    appInfo_2->setGeometry(400, 30, 130, 50);
    appInfo_2->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    appInfo_2->setFont(font_2);
    appInfo_2->setText("程序大小:");    // SerialNumber
    appLength = new QLabel(frame_setting);
    appLength->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:24px;}");
    appLength->setFont(font_2);
    appLength->setGeometry(530, 30, 200, 50);
    appLength->setFrameShape(QFrame::NoFrame);
    appLength->setText("-");
    updataFile = new QPushButton(frame_setting);
    updataFile->setStyleSheet("QPushButton { text-align:left; padding:5px; font-size:20px; border:1px solid #000000;}");
    updataFile->setFont(font);
    updataFile->setGeometry(10, 85, 500-4, 40);
//    updataFile->setFrameShape(QFrame::NoFrame);
    updataFile->setText("");
    connect(updataFile, &QAbstractButton::clicked, this, &RealTime::UpdataOpen);
    updataOpen = new QPushButton( " 打开文件", frame_setting);
    updataOpen->setGeometry(508, 85, 130, 40);
    updataOpen->setStyleSheet(bnt_qss2);
    updataOpen->setFont(font);
    connect(updataOpen, &QAbstractButton::clicked, this, &RealTime::UpdataOpen);
    updataBegin = new QPushButton( " 升级程序", frame_setting);
    updataBegin->setGeometry(656, 85, 130, 40);
    updataBegin->setStyleSheet(bnt_qss2);
    updataBegin->setFont(font);
    connect(updataBegin, &QAbstractButton::clicked, this, &RealTime::UpdataSend);
    updataBar = new QProgressBar(frame_setting);
    updataBar->setGeometry(10, 140, 780, 40);
    updataBar->setOrientation(Qt::Horizontal);  // 水平方向
    updataBar->setMinimum(0);  // 最小值
    updataBar->setMaximum(1000);  // 最大值
    updataBar->setValue(0);  // 当前进度
    double dProgress = (updataBar->value() - updataBar->minimum()) * 100.0
                    / (updataBar->maximum() - updataBar->minimum()); // 百分比计算公式
    updataBar->setFormat(QString("当前进度为：%1%").arg(QString::number(dProgress, 'f', 1)));
    updataBar->setFont(font_2);
//    updataBar->setFormat(tr("Current progress : %1%").arg(QString::number(dProgress, 'f', 1)));
    updataBar->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  // 对齐方式
    updataTips = new QLabel(frame_setting);
    updataTips->setStyleSheet("QLabel { text-align:left; padding:0px; font-size:18px;}");
    updataTips->setFont(font_2);
    updataTips->setGeometry(16, 140, 200, 40);
    updataTips->setFrameShape(QFrame::NoFrame);
    updataTips->setText("当前文件：");
//    updataTips->setVisible(false);
//    updataBar->setVisible(false);
    updataTips->setEnabled(false);
    updataBar->setEnabled(false);

    QLabel *averageInfo = new QLabel(frame_setting);
    averageInfo->setGeometry(10, 220, 400, 40);
    averageInfo->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    averageInfo->setFont(font_2);
    averageInfo->setText("平均值更新时间:        秒");
    averageValue = new QSpinBox(frame_setting);
    averageValue->setGeometry(200, 225, 80, 30);
    averageValue->setMinimum(1);
    averageValue->setMaximum(60);
    averageValue->setSingleStep(1);
    averageValue->setFont(font);
    averageValue->setValue(m_ComData->SettingAverageTime);
    connect(averageValue, SIGNAL(valueChanged(int)), SLOT(slotAverageValue(int)));

    QLabel *batteryCapacityInfo = new QLabel(frame_setting);
    batteryCapacityInfo->setGeometry(400, 220, 400, 40);
    batteryCapacityInfo->setStyleSheet("QLabel { text-align:left; padding:2px; font-size:24px;}");
    batteryCapacityInfo->setFont(font_2);
    batteryCapacityInfo->setText("电池容量:            mAh");
    batteryCapacity = new QSpinBox(frame_setting);
    batteryCapacity->setGeometry(520, 225, 120, 30);
    batteryCapacity->setMinimum(1000);
    batteryCapacity->setMaximum(50000);
    batteryCapacity->setSingleStep(1000);
    batteryCapacity->setFont(font);
    batteryCapacity->setValue(m_ComData->SettingBatteryCapacity);
    connect(batteryCapacity, SIGNAL(valueChanged(int)), SLOT(slotBatteryValue(int)));

    QFrame *frameTop = new QFrame(this);
    frameTop->setGeometry(800, 4, 275, 30);
//    frame->setStyleSheet("background-color:white");
    frameTop->setFrameShape(QFrame::NoFrame);
    play = new QPushButton(QIcon(":/play_red.png"), "继续", frame_2);
    play->setGeometry(1145 - 360, 15, 80, 25);
    play->setStyleSheet(bnt_qss1);
    play->setFont(font);
    play->setVisible(false);
    connect(play, &QAbstractButton::clicked, this, &RealTime::onBtnPlay);
    pause = new QPushButton(QIcon(":/pause.png"), "暂停", frame_2);
    pause->setGeometry(1225 - 360, 15, 80, 25);
    pause->setStyleSheet(bnt_qss1);
    pause->setFont(font);
    pause->setVisible(false);
    connect(pause, &QAbstractButton::clicked, this, &RealTime::onBtnPause);

    FixCurrentScale = new QComboBox(frame_2);
    FixCurrentScale->setGeometry(250, 20, 120, 20);
    FixCurrentScale->addItems(QStringList() << "自动量程" << "3000mA" << "1000mA" << "100mA" << "10mA" << "1mA" << "100uA" << "10uA" << "1uA");
    connect(FixCurrentScale, SIGNAL(currentIndexChanged(int)), this, SLOT(slotFixCurrentScale(int)));
    fixCurrentValue = 0;

//    // 保存按键代码
//    download = new QPushButton(QIcon(":/save.png"), "导出", frameTop);
//    download->setGeometry(195, 4, 80, 30);
//    download->setStyleSheet(bnt_qss1);
//    download->setFont(font_2);
//    connect(download, &QAbstractButton::clicked, this, &RealTime::onBtnDownload);

    // Can start now
//    updatePeriod->setCurrentIndex(0);
//    runPB->click();

    SendVerifyCmd = new QTimer(this);
    connect(SendVerifyCmd, SIGNAL(timeout()), this, SLOT(slotSendVerifyCmd()));
    SendVerifyCount = 0;
}

RealTime::~RealTime()
{
    qDebug() << "RealTime析构函数。";
    delete m_ChartViewer->getChart();
    delete m_ChartViewer_2->getChart();

    delete m_UsbReceiveThread;
    delete m_UsbSendThread;
    delete m_SqliteThread;
//    delete m_About;
//    delete m_UsbHid;
//    if(m_ComData != nullptr)
//    {
//        delete m_ComData;
//        m_ComData = nullptr;
//    }
}

void RealTime::resizeEvent(QResizeEvent *event)
{
     qDebug() << "RealTime::frameGeometry().size() = " << frameGeometry().size();
//     this->resize(frameGeometry().size()); //是采用size()还是frameGeometry.size()根据自己的需求。
//     updateChart();
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
    // 显示电流/电压/功率
    showVAW(m_ComData->d_dataSeriesV[m_ComData->d_currentIndex - 1], m_ComData->d_dataSeriesA[m_ComData->d_currentIndex - 1]);
//    m_ValueC->setText("888.888");
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
// Draw track cursor when mouse is moving over plotarea
//
void RealTime::onMouseClick(QMouseEvent *)
{
    qDebug() << "鼠标点击事件" << m_ChartViewer->getPlotAreaMouseX();
//    trackLineLabel_T1Or2((XYChart *)m_ChartViewer->getChart(), m_ChartViewer->getPlotAreaMouseX(), 0, 0);
//    m_ChartViewer->updateDisplay();
}
void RealTime::onMouseClick_2(QMouseEvent *)
{
//    trackLineLabel_T1Or2((XYChart *)m_ChartViewer_2->getChart(), m_ChartViewer_2->getPlotAreaMouseX(), 1, 0);
//    m_ChartViewer_2->updateDisplay();
}
void RealTime::onMouseMovePlotArea(QMouseEvent *)
{
    trackLineLabel((XYChart *)m_ChartViewer->getChart(), m_ChartViewer->getPlotAreaMouseX(), 0);
    m_ChartViewer->updateDisplay();
}
void RealTime::onMouseMovePlotArea_2(QMouseEvent *)
{
    trackLineLabel((XYChart *)m_ChartViewer_2->getChart(), m_ChartViewer_2->getPlotAreaMouseX(), 1);
    m_ChartViewer_2->updateDisplay();
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
    XYChart *c = new XYChart(1345 - 360 + 10, 425 - 40);     // 1345, 425        m_HScrollBar->width(), 300

    // Set the plotarea at (55, 50) with width 80 pixels less than chart width, and height 80 pixels
    // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
    // as background. Set border to transparent and grid lines to white (ffffff).
    c->setPlotArea(55, 62, c->getWidth() - 85, c->getHeight() - 100, c->linearGradientColor(0, 50, 0,
        c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

    // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
    c->setClipping();

    // Add a title to the chart using 18pt Arial font
//    if(index == 0)
//        c->addTitle("Voltage with Zoom/Scroll and Track Line", "arial.ttf", 18);
//    else if (index == 1)
//        c->addTitle("Current with Zoom/Scroll and Track Line", "arial.ttf", 18);

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
        c->yAxis()->setTitle("Voltage ( V )", "arialbd.ttf", 12);
    else if (index == 1)
        c->yAxis()->setTitle("Current ( mA )", "arialbd.ttf", 12);


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
    char buffer[1024];
    if(index == 0) {
        if(m_ComData->d_currentIndex > 0) {
            sprintf(buffer, " <*bgColor=ffffff*> <*color=00cc00*> <*size=14px*> %.3f V", m_ComData->d_dataSeriesV[m_ComData->d_currentIndex - 1]);
//        layer->addDataSet(DoubleArray(m_ComData->d_dataSeriesV, m_ComData->d_currentIndex), 0x00cc00, buffer);
        }
        layer->addDataSet(viewPortDataSeriesB, 0x00cc00, buffer);
        c->yAxis()->setMinTickInc(0.1);
        c->yAxis()->setDateScale(0, 7.5);           // 固定坐标轴0-7.5V
    }
    else if(index == 1) {
        if(m_ComData->d_currentIndex > 0) {
            double d = m_ComData->d_dataSeriesA[m_ComData->d_currentIndex - 1];
            if(d < 1) {
                sprintf(buffer, " <*bgColor=ffffff*> <*color=0000ff*> <*size=14px*> %.3f uA", d * 1000);
            } else {
                sprintf(buffer, " <*bgColor=ffffff*> <*color=0000ff*> <*size=14px*> %.2f mA", d);
            }
    //        layer->addDataSet(DoubleArray(m_ComData->d_dataSeriesA, m_ComData->d_currentIndex), 0x00ff, buffer);
        }
        layer->addDataSet(viewPortDataSeriesC, 0x0000ff, buffer);
//        c->yAxis()->setMinTickInc(0.1);
        if(fixCurrentValue > 0)
            c->yAxis()->setDateScale(0, fixCurrentValue);
    }

    //================================================================================
    // Configure axis scale and labelling
    //================================================================================

    // Set the x-axis as a date/time axis with the scale according to the view port x range.
    if (m_ComData->d_currentIndex > 0)
        c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

    // For the automatic axis labels, set the minimum spacing to 75/30 pixels for the x/y axis.
    c->xAxis()->setTickDensity(75);
    c->yAxis()->setTickDensity(30);
//    c->yAxis()->setLinearScale(0, 1000, 100, 10);
//    c->yAxis()->setTickLength(8, 4);
//    c->yAxis()->setLength(1);
//    c->yAxis()->setAutoScale();

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
    c->xAxis()->setMinTickInc(0.001);

    //================================================================================
    // Output the chart
    //================================================================================

    if(m_ComData->d_currentIndex > 2000)
    {
        c->xAxis()->addMark(m_ComData->d_timeStamps[1000], 0xFF4500, "T1 = 1000")->setLineWidth(2);
        c->xAxis()->addMark(m_ComData->d_timeStamps[2000], 0xF4A460, "T2 = 2000")->setLineWidth(2);
    }


//    T1_Cur_Index = 1000;
//    T2_Cur_Index = 2000;
//    if(m_ComData->d_currentIndex > 2000 && index == 1 && m_ComData->d_currentIndex < 10000)
//    {
//        LineLayer *layer_T1 = c->addLineLayer();
//        layer_T1->setLineWidth(2);
//        layer_T1->setFastLineMode();

//        DoubleArray T1_XArr;
//        DoubleArray T1_YArr;
//        T1_XArr = DoubleArray(m_ComData->d_timeStamps + 1000, 2);
//        double buf[2] = {0, 0.00008};      //
//        T1_YArr = DoubleArray(buf, 2);
//        qDebug() << "显示T1_XArr" << QString::number(T1_XArr[0]) << QString::number(T1_XArr[1]);
//        qDebug() << "显示T1_YArr" << QString::number(T1_YArr[0]) << QString::number(T1_YArr[1]);

//        layer_T1->setXData(T1_XArr);
//        char buffer[1024];
//        layer_T1->addDataSet(T1_YArr, 0xFF0000, buffer);

//    }

    // We need to update the track line too. If the mouse is moving on the chart (eg. if
    // the user drags the mouse on the chart to scroll it), the track line will be updated
    // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
    if (!viewer->isInMouseMoveEvent())
    {
        trackLineLabel(c, (0 == viewer->getChart()) ? c->getPlotArea()->getRightX() :
            viewer->getPlotAreaMouseX(), index);
    }
    // Set the chart image to the QChartViewer
    delete viewer->getChart();
    viewer->setChart(c);
}

//
// Draw the track line with data point labels
//
void RealTime::trackLineLabel(XYChart *c, int mouseX, int index)
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
    xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss.fff")
        << " <*/font*>";
    TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 10);       //         //font_2.family().toLatin1().data()

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
                if(index == 0) {
                    label << "<*font,bgColor=" << hex << color << "*> "
                        << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << "V" << " <*font*>";
                } else if(index == 1) {
                    double bufD = dataSet->getValue(xIndex);
                    if(bufD < 1) {
                        label << "<*font,bgColor=" << hex << color << "*> "
                            << c->formatValue(bufD * 1000, "{value|P4}") << "uA" << " <*font*>";
                    } else {
                        label << "<*font,bgColor=" << hex << color << "*> "
                            << c->formatValue(bufD, "{value|P4}") << "mA" << " <*font*>";
                    }
                }


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

void RealTime::trackLineLabel_T1Or2(XYChart *c, int mouseX, int index, int T1Or2)
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

//    if(index == 1)
//    {
//        qDebug() << "plotArea->getLeftX() = " << plotArea->getLeftX();          // 55
//        qDebug() << "plotArea->getRightX() = " << plotArea->getRightX();        // 965
//        qDebug() << "xValue = " << xValue;
//        qDebug() << "xCoor = " << xCoor;
//    }
    // Draw a vertical track line at the x-position
    d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0xFF0000);     // 0x888888

    // Draw a label on the x-axis to show the track line position.
    ostringstream xlabel;
    xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "T=hh:nn:ss.fff")
        << " <*/font*>";

//    if(index == 1)
//    {
//        qDebug() << "m_ComData->d_currentIndex = " << m_ComData->d_currentIndex;
//        qDebug() << "最左边的索引：" << c->getLayer(0)->getXIndexOf(c->getXValue(plotArea->getLeftX()));
//        qDebug() << "当前的索引" << c->getLayer(0)->getXIndexOf(xValue);
////        qDebug() << "c->getXValue = " << c->getXValue(xCoor);
//    }
    TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 10);       //         //font_2.family().toLatin1().data()

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
                if(index == 0) {
                    label << "<*font,bgColor=" << hex << color << "*> "
                        << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << "V" << " <*font*>";
                } else if(index == 1) {
                    double bufD = dataSet->getValue(xIndex);
                    if(bufD < 1) {
                        label << "<*font,bgColor=" << hex << color << "*> "
                            << c->formatValue(bufD * 1000, "{value|P4}") << "uA" << " <*font*>";
                    } else {
                        label << "<*font,bgColor=" << hex << color << "*> "
                            << c->formatValue(bufD, "{value|P4}") << "mA" << " <*font*>";
                    }
                }


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
//    if (m_ComData->d_currentIndex > 0)
//    {
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
//    }
}

void RealTime::onConnectUSB()
{
//    writeSQL(0, 0, 0);
//    return;

    if(m_UsbHid->ConnectUSB())
    {
        connectUSB->setEnabled(false);
        disconnectUSB->setEnabled(true);
        qDebug() << "连接成功";

        emit CreateSqilite();
        m_SqliteThread->isStop = false;
        m_SqliteThread->start();

            m_UsbReceiveThread->isStop = false;
            m_UsbReceiveThread->start();   // 启动线程
            m_ChartUpdateTimer->start(100);    // 启动更新表格
//            m_ComData->ClearData();         // 清之前的数据
            play->setVisible(true);
            play->setEnabled(false);
            pause->setVisible(true);
            pause->setEnabled(true);
            usb_str1->setText(m_UsbHid->str_Manufactured);
            usb_str2->setText(m_UsbHid->str_Product);
            usb_str3->setText(m_UsbHid->str_SerialNumber);
            m_Tips->setText("");
            m_Error->setText("");
            buf1_QL->setText("-");  // 初始化显示-
            buf2_QL->setText("-");  // 初始化显示-
            buf3_QL->setText("-");  // 初始化显示-
            bRunningTimeHour->setText(" -");
            bRunningTimeMinute->setText(" -");
            bRemainTimeHour->setText(" -");
            bRemainTimeMinute->setText(" -");
            bRemainCap->setText("100%");
            m_Energy->setText(QString::number(0, 'f', 2));  // 初始化显示0
    //        download->setEnabled(false);
            // 设置更新进度条不可见
    //        updataTips->setVisible(false);
    //        updataBar->setVisible(false);
            updataTips->setEnabled(false);
            updataBar->setEnabled(false);
            // 发送读取版本号和文件长度指令
//
            historyView->ClearData();       // 清除历史数据
//            historyDetail->ClearData();

            SendVerifyCmd->start(100);
            SendVerifyCount = 0;
//            send_CMD(0x20);     // 读取各个档位的校验值
            averageValue->setEnabled(false);
            batteryCapacity->setEnabled(false);
            tabWidget->setCurrentIndex(0);  // 跳到第一页
   //     }
    }
    else
    {
        qDebug() << "连接失败";
        QMessageBox::critical(this, "提示", "连接失败！");
    }
}
void RealTime::onDisConnectUSB()
{
    if(m_UsbSendThread->isStop == false)
    {
        qDebug() << "关闭失败";
        QMessageBox::critical(this, "提示", "正在更新程序，请勿关闭！");
        return;
    }
    send_CMD(0x21);
    m_UsbReceiveThread->isStop = true;
    m_SqliteThread->isStop = true;
    m_SqliteThread->terminate();
//    m_UsbReceiveThread->terminate();    // 关闭线程
//    m_UsbReceiveThread->wait();
    m_UsbSendThread->isStop = true;
    m_UsbSendThread->terminate();       // 关闭发送线程

    m_ChartUpdateTimer->stop();     // 关闭更新表格
    usb_str1->setText("-");
    usb_str2->setText("-");
    usb_str3->setText("-");

    averageValue->setEnabled(true);
    batteryCapacity->setEnabled(true);
    historyView->ClearData();       // 清除历史数据
//    historyDetail->ClearData();
}

void RealTime::thread_receive_finished()
{
    if(m_UsbHid->DisConnectUSB())
    {
        connectUSB->setEnabled(true);
        disconnectUSB->setEnabled(false);

//        m_UsbReceiveThread->terminate();    // 关闭线程
        play->setVisible(false);
        pause->setVisible(false);
//        download->setEnabled(true);

        qDebug() << "关闭成功";
    }
    else
    {
        qDebug() << "关闭失败";
        QMessageBox::critical(this, "提示", "关闭失败！");
    }
}
void RealTime::thread_send_finished()
{
    m_UsbSendThread->isStop = true;
    m_UsbSendThread->terminate();       // 关闭发送线程
    qDebug() << "发送线程关闭成功";
}
void RealTime::thread_sqlite_finished()
{
    m_SqliteThread->isStop = true;
    m_SqliteThread->terminate();       // 关闭发送线程
    qDebug() << "写数据库线程关闭成功";
}
/*
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
*/
void RealTime:: CreateData()
{
    return;
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

void RealTime::m_get_USB_Data(QDateTime now, double tep, unsigned char tips, unsigned char err)
{
    if(err != 0)
        m_Error->setText("错误代码：" + QString::number(err));
    else {
        m_Temp->setText("设备内部温度:" + QString::number(tep, 'f', 1) + "℃");
        if(tips == 1)
            m_Tips->setText("提示：输入电流太小！");
        else if(tips == 2)
            m_Tips->setText("提示：输入电流过大！");
        else
            m_Tips->setText("");
        // 显示波形和Tab
        showVAW(m_ComData->d_dataSeriesV[m_ComData->d_currentIndex - 1], m_ComData->d_dataSeriesA[m_ComData->d_currentIndex - 1]);
    }
}

void RealTime::m_get_Version_Length(unsigned long long ver, unsigned long long len)
{
    unsigned char version[4];
    memcpy(version,  &ver, 4);   // 赋值版本号
    qDebug() << "版本号为：" << version[0] << version[1] << version[2] << version[3];
    qDebug() << "文件长度为：" << len;
    QString strVersion = QString(version[0]) + QString::number(version[1]) + "." + QString::number(version[2]) + "." + QString::number(version[3]);
    QString strLength = QString::number(len/1024) + "k";
    appVersion->setText(strVersion);
    appLength->setText(strLength);
}

void RealTime::showVAW(double v, double mA)
{
    cntDisplay++;
    if(cntDisplay >= 500)
    {
        cntDisplay = 0;     // 没10个数据更新一次显示，大约100ms
        // 更新显示
        m_ValueB->setText(QString::number(v, 'f', 3));
        if(mA < 1) {
            m_ValueC->setText(QString::number(mA * 1000, 'f', 3));
            m_unitA->setText("uA");
        } else if(mA >= 1 && mA < 1000) {
            m_ValueC->setText(QString::number(mA, 'f', 3));
            m_unitA->setText("mA");
        } else if(mA >= 1000) {
            m_ValueC->setText(QString::number(mA, 'f', 2));
            m_unitA->setText("mA");
        }
        double bufPower = v * mA / 1000;
        m_Power->setText(QString::number(bufPower, 'f', 2));
    }
}

void RealTime::slotShowEnergy(void)
{
    m_Energy->setText(QString::number(m_ComData->d_calculateValue.energySum, 'f', 2));
    bRemainCap->setText(QString::number((m_ComData->SettingBatteryCapacity - m_ComData->d_calculateValue.energySum) / m_ComData->SettingBatteryCapacity * 100, 'f', 2) + "%");
}
void RealTime::showAverage(void)
{
    // 更新平均值显示
    buf1_QL->setText(QString::number(m_ComData->d_Avg_V, 'f', 3));
    if(m_ComData->d_Avg_A < 1) {
        buf2_QL->setText(QString::number(m_ComData->d_Avg_A * 1000, 'f', 3));
        m_averageA->setText("uA");
    } else {
        buf2_QL->setText(QString::number(m_ComData->d_Avg_A, 'f', 3));
        m_averageA->setText("mA");
    }
    double bufPower_2 = m_ComData->d_Avg_V * m_ComData->d_Avg_A / 1000;
    buf3_QL->setText(QString::number(bufPower_2, 'f', 3));

}
void RealTime::slotShowTime(qint64 runningT, qint64 remainT)
{
    bRunningTimeHour->setText(QString::number(runningT / 60));
    bRunningTimeMinute->setText(QString::number(runningT % 60));
    if(runningT % 60 < 10)
        bRunningTimeMinute->setGeometry(197 + 15, 90 + 8, 40, 40);
    else
        bRunningTimeMinute->setGeometry(197, 90 + 8, 40, 40);
    if(runningT / 60 >= 100)
        bRunningTimeHour->setGeometry(120, 90 + 8, 80, 40);
    else if(runningT / 60 < 10)
        bRunningTimeHour->setGeometry(120 + 30, 90 + 8, 80, 40);
    else
        bRunningTimeHour->setGeometry(120 + 15, 90 + 8, 80, 40);

    if(remainT > 60000 - 1)
        remainT = 60000 - 1;
    bRemainTimeHour->setText(QString::number(remainT / 60));
    bRemainTimeMinute->setText(QString::number(remainT % 60));
    if(remainT % 60 < 10)
        bRemainTimeMinute->setGeometry(197 + 15, 120 + 8, 40, 40);
    else
        bRemainTimeMinute->setGeometry(197, 120 + 8, 40, 40);
    if(remainT / 60 >= 100)
        bRemainTimeHour->setGeometry(120, 120 + 8, 80, 40);
    else if(remainT / 60 < 10)
        bRemainTimeHour->setGeometry(120 + 30, 120 + 8, 80, 40);
    else
        bRemainTimeHour->setGeometry(120 + 15, 160 + 8, 80, 40);
}

void RealTime::linkUs(QString str)
{
//    qDebug() << "联系我们" << str;
    if(m_About == nullptr)
    {
        QDesktopWidget* desktopWidget = QApplication::desktop();
        QRect clientRect = desktopWidget->availableGeometry();

        m_About = new About(nullptr);
//        demo->setStyleSheet("* {font-family:arial;font-size:15px}");
//        demo->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // 置顶，最小化，关闭
        m_About->setGeometry(clientRect.size().width() / 2 - 300,clientRect.size().height() / 2 - 120, 600, 240);
        m_About->show();
        connect(m_About,SIGNAL(destroyed()),this, SLOT(aboutClose()));
    }
}

void RealTime::aboutClose(void)
{
//    qDebug() << "About关闭。";
    m_About = nullptr;
}

QString RealTime::loadFontFamilyFromTTF(QString str)
{
    static QString font;
    static bool loaded = false;
    if(!loaded)
    {
        loaded = true;
        int loadedFontID = QFontDatabase::addApplicationFont(str);
        QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
        if(!loadedFontFamilies.empty())
            font = loadedFontFamilies.at(0);
    }
    return font;
}

void RealTime::onBtnPlay()
{
    if(m_UsbHid->dev_handle == nullptr)
    {
//        qDebug() << "USB设备未打开！";
        return;
    }
    if(m_UsbSendThread->isStop == false)
    {
        qDebug() << "点击开始失败";
        QMessageBox::critical(this, "提示", "正在更新程序，请勿执行其他操作！");
        return;
    }
    play->setEnabled(false);
    pause->setEnabled(true);
    m_ChartUpdateTimer->start(100);    // 启动更新表格
}
void RealTime::onBtnPause()
{
    if(m_UsbHid->dev_handle == nullptr)
    {
//        qDebug() << "USB设备未打开！";
        return;
    }
    if(m_UsbSendThread->isStop == false)
    {
        qDebug() << "点击暂停失败";
        QMessageBox::critical(this, "提示", "正在更新程序，请勿执行其他操作！");
        return;
    }
    play->setEnabled(true);
    pause->setEnabled(false);
    m_ChartUpdateTimer->stop();    // 关闭更新表格
}
void RealTime::onBtnDownload()
{
//    qDebug() << "点击导出按键。";
    if(m_ComData->d_currentIndex <= 1)
    {
        QMessageBox::critical(this, "提示", "暂无数据，无法导出！");
        return;
    }
    if(m_UsbSendThread->isStop == false)
    {
        qDebug() << "点击保存失败";
        QMessageBox::critical(this, "提示", "正在更新程序，请勿执行其他操作！");
        return;
    }
    QString sFilePath="data.txt";
    QFile file(sFilePath);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QMessageBox::critical(this,"提示","无法创建文件！");
        return;
    }
    QTextStream out(&file);
    QString title = "序号\t时间\t电压\t电流\t功率";
    out<<title<<endl;//将a存进test.txt文件夹里面<<QObject::tr("go on")
    for (int i = 0; i < m_ComData->d_currentIndex; i++) {
        QString v = QString::number(m_ComData->d_dataSeriesV[i], 'f', 3) + "V";
        QString a;
        if(m_ComData->d_dataSeriesA[i] < 1) {
            a = QString::number(m_ComData->d_dataSeriesA[i] * 1000, 'f', 3) + "uA";
        } else {
            a = QString::number(m_ComData->d_dataSeriesA[i], 'f', 3) + "mA";
        }
        out<<QString::number(i + 1)<<"\t"<<doubleToTime(m_ComData->d_timeStamps[i])<<"\t"<<v<<"\t"<<a<<"\t"<<QString::number(m_ComData->d_dataSeriesV[i] * m_ComData->d_dataSeriesA[i], 'f', 3) + "W"<<endl;
    }

    out.flush();
    file.close();
    QMessageBox::about(this,"提示","导入数据到文件'data.txt'成功！");
}

QString RealTime::doubleToTime(double dTime)
{
    QDateTime *dt = new QDateTime();
    dt->setDate(m_ComData->lastTime.date());
    dt->setTime(m_ComData->lastTime.time());
    double dec = dTime - m_ComData->d_timeStamps[m_ComData->d_currentIndex - 1];
//    *dt = dt->addSecs((qint64)floor(dec));
    *dt = dt->addMSecs((qint64)round(dec * 1000));
    QString ret = dt->toString("yyyy-MM-dd hh:mm:ss");
    delete dt;
    return ret;
}

void RealTime::UpdataOpen()
{
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        QMessageBox::about(this, "提示", "USB设备未打开！");
        return;
    }
    if(m_UsbSendThread->isStop == false)
    {
        qDebug() << "点击打开失败";
        QMessageBox::critical(this, "提示", "正在更新程序，请勿执行其他操作！");
        return;
    }

    QString fileName=QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("bin file"),qApp->applicationDirPath(),
                                                  QString::fromLocal8Bit("bin File(*.bin)"));//新建文件打开窗口
    if (fileName.isEmpty())//如果未选择文件便确认，即返回
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        qDebug() << file.errorString();//文件打开错误显示错误信息
    QByteArray arry=file.readAll();//读取文件
    file.close();
    qDebug() << "打开文件：" << fileName;
//    int length=arry.size();//计算长度    qDebug()<<length;

#ifndef appUpdataDebug
    if((arry.size() % 1024) != 4 || arry[arry.size() - 4] != 'V')
    {
        qDebug() << "升级文件有误，无法加载！";
        QMessageBox::about(this, "提示", "升级文件有误，无法加载！");
        return;
    }
    for (int i = 0; i < arry.size(); i++)
    {
       arry.data()[i]  = (byte)((int)arry.data()[i] ^ (i % 256));
    }
#else
    qDebug() << "1.arry.size() = " << arry.size();
    if((arry.size() % 1024) != 0) {
            arry.insert(arry.size(), 1024 - (arry.size() % 1024), -1);
    }
//    length = arry.size();//计算长度
    qDebug() << "2.arry.size() = " << arry.size();
#endif

    if(arry.size() > 122000 + 4)
    {
        qDebug() << "升级文件过大，无法加载！";
        QMessageBox::about(this, "提示", "升级文件过大，无法加载！");
        return;
    }
    updataFile->setText(fileName);
    updataTips->setText("当前文件：" + QString::number(arry.size() - 4) + "字节");
//    updataTips->setVisible(true);
    updataTips->setEnabled(true);
    updataBar->setMaximum(arry.size() - 4);
//    updataBar->setVisible(true);
    updataBar->setEnabled(true);
    updataBar->setValue(0);  // 当前进度
    m_ComData->updataFileLen = (unsigned int)arry.size() - 4;     // 赋值长度,去掉最后4个字节（版本号）
//    m_ComData->updataFile = new unsigned char[m_ComData->updataFileLen];
//    m_ComData->updataFile = (unsigned char *)arry.data();
//    memcpy(m_ComData->updataFile, arry.data(), m_ComData->updataFileLen);
    for (int i = 0; i < arry.size(); i++) {
//        qDebug("%x ", *(arry.data() + i));
        m_ComData->updataFile[i] = (unsigned char)(*(arry.data() + i));
    }

    /*
    QString fileName = QFileDialog::getOpenFileName(
                           this,
                           tr("Select KW35 Bin file"),
                           "./",
                           tr("bin file(*.bin)"));
   if(fileName.isEmpty()){
       QMessageBox mesg;
       mesg.critical(this,tr("Error"),tr("打开Bin文件发生错误！"));
       return;
   }else{
       qDebug() << "打开文件：" << fileName;
   }
   QFileInfo *fileInfo = new QFileInfo(fileName);
   qDebug() << "文件长度：" << fileInfo->size();

   QFile *file = new QFile;
   file->setFileName(mFileInfo.BinFileInfo.filePath());
   if(file->open(QIODevice::ReadOnly)){
       QDataStream BinFileData(file);
       char *pBuff = new char[mFileInfo.BinFileInfo.size()];
       BinFileData.readRawData(pBuff,static_cast<int>(mFileInfo.BinFileInfo.size()));
       mFileInfo.BinFileRawData = QByteArray(pBuff,static_cast<int>(mFileInfo.BinFileInfo.size()));

       //qDebug()<< mFileInfo.BinFileRawData;
       file->close();
   }else {
       QMessageBox mesg;
       mesg.critical(this,tr("Error"),tr("无法读取,请检查BIN文件路径!"));
       return;
   }
*/
}
void RealTime::UpdataSend()
{
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        QMessageBox::about(this, "提示", "USB设备未打开！");
        return;
    }
    if(m_UsbSendThread->isStop == false)
    {
        qDebug() << "点击更新失败";
        QMessageBox::critical(this, "提示", "正在更新程序，请勿执行其他操作！");
        return;
    }

    m_UsbSendThread->isStop = false;
    m_UsbSendThread->start();   // 启动线程

}

void RealTime::updataProgressBar(unsigned long val)
{
    updataBar->setValue((int)val);  // 当前进度
    double dProgress = (updataBar->value() - updataBar->minimum()) * 100.0
                    / (updataBar->maximum() - updataBar->minimum()); // 百分比计算公式
    updataBar->setFormat(QString("当前进度为：%1%").arg(QString::number(dProgress, 'f', 1)));

}

void RealTime::upadataSuccess()
{
    m_UsbSendThread->isStop = true;
    m_UsbSendThread->terminate();       // 关闭发送线程
    updataBar->setValue(updataBar->maximum());  // 设为100%
    updataBar->setFormat(QString("当前进度为：%1%").arg(QString::number(100, 'f', 1)));
    onDisConnectUSB();      // 关闭USB设备
    QMessageBox::about(this, "提示", "升级成功，请重启设备电源！");
}
void RealTime::updataFail()
{
    m_UsbSendThread->isStop = true;
    m_UsbSendThread->terminate();       // 关闭发送线程
    onDisConnectUSB();      // 关闭USB设备
    QMessageBox::about(this, "提示", "升级失败，请重新升级程序！");
}
void RealTime::upadtaTimeOut()
{
    m_UsbSendThread->isStop = true;
    m_UsbSendThread->terminate();       // 关闭发送线程
    onDisConnectUSB();      // 关闭USB设备
    QMessageBox::about(this, "提示", "升级超时，请重新升级程序！");
}

void RealTime::send_CMD(unsigned char cmd)
{
    if(m_UsbHid->dev_handle == nullptr)
    {
        qDebug() << "USB设备未打开！";
        return;
    }
    unsigned char sendP[32];
    memset(sendP, 0, sizeof (sendP));
    sendP[0] = 0xa5; sendP[1] = 0xb7; sendP[2] = 0xa5; sendP[3] = 0xb7;
    sendP[4] = cmd;
    m_UsbHid->SendUSB(sendP, 32);   // 使用USB发送数据
}

void RealTime::writeSQL(qint64 time, double vol, double cur)
{


}

/*
void RealTime::writeSQL(qint64 time, double vol, double cur)
{

    m_ComData->d_calculateValue.currentSumSecond += cur;         // mA
    if(0 == m_ComData->RunningCount % 1000)
    {
        m_ComData->d_calculateValue.energySum += (m_ComData->d_calculateValue.currentSumSecond / 1000) / 3600;
        m_ComData->d_calculateValue.currentSumSecond = 0;
        m_Energy->setText(QString::number(m_ComData->d_calculateValue.energySum, 'f', 2));
        if(m_ComData->d_calculateValue.energySum >= m_ComData->SettingBatteryCapacity * 0.8)
            m_ComData->d_calculateValue.energySum = m_ComData->SettingBatteryCapacity * 0.8;
        bRemainCap->setText(QString::number((m_ComData->SettingBatteryCapacity * 0.8 - m_ComData->d_calculateValue.energySum) / m_ComData->SettingBatteryCapacity * 125, 'f', 2) + "%");
    }

    m_ComData->d_calculateValue.voltageSum += vol;  m_ComData->d_calculateValue.currentSum += cur;
    if(++m_ComData->d_calculateValue.countSum >= 60000)
    {
        m_ComData->d_calculateValue.voltageSum /= m_ComData->d_calculateValue.countSum; m_ComData->d_calculateValue.currentSum /= m_ComData->d_calculateValue.countSum; m_ComData->d_calculateValue.countSum = 0;
        m_ComData->AverageMinuteCount++;
        if(m_ComData->AverageMinuteCount >= 60)
        {
            m_ComData->AverageMinuteCount = 60;
            ComData::shiftData_D(m_ComData->AverageVolMinute, sizeof(m_ComData->AverageVolMinute) / sizeof(double), m_ComData->d_calculateValue.voltageSum);
            ComData::shiftData_D(m_ComData->AverageCurMinute, sizeof(m_ComData->AverageCurMinute) / sizeof(double), m_ComData->d_calculateValue.currentSum);
        }
        else
        {
            m_ComData->AverageVolMinute[m_ComData->AverageMinuteCount - 1] = m_ComData->d_calculateValue.voltageSum;
            m_ComData->AverageCurMinute[m_ComData->AverageMinuteCount - 1] = m_ComData->d_calculateValue.currentSum;
        }
        m_ComData->d_calculateValue.voltageSum = 0;  m_ComData->d_calculateValue.currentSum = 0;
        // 更新平均值UI
        if(m_ComData->SettingAverageTime <= m_ComData->AverageMinuteCount)
        {
            double bufV = 0, bufA = 0;
            for (int i = m_ComData->AverageMinuteCount - m_ComData->SettingAverageTime; i < m_ComData->AverageMinuteCount; i++) {
                bufV += m_ComData->AverageVolMinute[i];
                bufA += m_ComData->AverageCurMinute[i];
            }
            m_ComData->d_Avg_V = bufV / m_ComData->SettingAverageTime;
            m_ComData->d_Avg_A = bufA / m_ComData->SettingAverageTime;
            showAverage();
        }
        // 更新电池信息UI
//         qDebug() << "m_ComData->RunningCount" << m_ComData->RunningCount;
        qint64 runningMinute =  m_ComData->RunningCount / 60000;
        bRunningTimeHour->setText(QString::number(runningMinute / 60));
        bRunningTimeMinute->setText(QString::number(runningMinute % 60));
        if(runningMinute % 60 < 10)
            bRunningTimeMinute->setGeometry(197 + 15, 90 + 8, 40, 40);
        else
            bRunningTimeMinute->setGeometry(197, 90 + 8, 40, 40);
        if(runningMinute / 60 >= 100)
            bRunningTimeHour->setGeometry(120, 90 + 8, 80, 40);
        else if(runningMinute / 60 < 10)
            bRunningTimeHour->setGeometry(120 + 30, 90 + 8, 80, 40);
        else
            bRunningTimeHour->setGeometry(120 + 15, 90 + 8, 80, 40);
        qint64 remainMinute = 0;
        double curAvg = 0;
        for (int i = 0; i < m_ComData->AverageMinuteCount; i++) {
            curAvg += m_ComData->AverageCurMinute[i];
        }
        curAvg /= m_ComData->AverageMinuteCount;
        remainMinute = (m_ComData->SettingBatteryCapacity * 0.8 - m_ComData->d_calculateValue.energySum) / curAvg * 60;
        if(remainMinute > 60000 - 1)
            remainMinute = 60000 - 1;
        bRemainTimeHour->setText(QString::number(remainMinute / 60));
        bRemainTimeMinute->setText(QString::number(remainMinute % 60));
        if(remainMinute % 60 < 10)
            bRemainTimeMinute->setGeometry(197 + 15, 120 + 8, 40, 40);
        else
            bRemainTimeMinute->setGeometry(197, 120 + 8, 40, 40);
        if(remainMinute / 60 >= 100)
            bRemainTimeHour->setGeometry(120, 120 + 8, 80, 40);
        else if(remainMinute / 60 < 10)
            bRemainTimeHour->setGeometry(120 + 30, 120 + 8, 80, 40);
        else
            bRemainTimeHour->setGeometry(120 + 15, 160 + 8, 80, 40);
    }

//     qDebug() << "测试数据库读写。" << QDir::currentPath();

//     QDateTime timeQ = QDateTime::currentDateTime();   //获取当前时间
//     int64_t timeD = timeQ.toMSecsSinceEpoch();     //将当前时间转为时间戳,精确到ms
//     int timeT = timeQ.toTime_t();   //将当前时间转为时间戳
//     qDebug() << "timeQ = " << timeQ << "; timeT = " << timeT << "; timeD = " << timeD;
//     QDateTime timeGet = QDateTime::fromTime_t(timeT);
//     qDebug() << "timeGet = " << timeGet;
//     QDateTime timeGet_M = QDateTime::fromMSecsSinceEpoch(timeD);
//     qDebug() << "timeGet_M = " << timeGet_M;

//         query.prepare("INSERT INTO stm32_data (time, voltage, current) "
//                           "VALUES (:time, :voltage, :current)");   //为每一列标题添加绑定值
//         for(int i = 0 ; i < 10; i++)       //从names表里获取每个名字
//         {
//             query.bindValue(":time", QDateTime::currentDateTime().toMSecsSinceEpoch());                      //向绑定值里加入
//             query.bindValue(":voltage", (double)i/1000);      //
//             query.bindValue(":current", (double)i/2000);    //
//             query.exec();               //加入库中
//          }
//        qDebug() << "插入数据前时间" << QDateTime::currentDateTime();
//        query.exec("INSERT INTO stm32_data (ID,time,voltage,current) VALUES (20, 100, 100.01, 20000.00 )");     // 写一条指令时间8ms~9ms
//        qDebug() << "插入数据后时间" << QDateTime::currentDateTime();

}
*/
void RealTime::HistoryOpen()
{
    if(m_UsbHid->dev_handle != nullptr)
    {
        qDebug() << "USB正在运行，无法加载文件！";
         QMessageBox::critical(this, "提示", "USB正在运行，无法加载文件！");
        return;
    }
    qDebug() << qApp->applicationDirPath();
    QString fileName=QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("历史数据"),qApp->applicationDirPath() + "/iSCAN_Data",
                                                  QString::fromLocal8Bit("bin File(*.db)"));//新建文件打开窗口
    if (fileName.isEmpty())//如果未选择文件便确认，即返回
        return;
//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly))
//        qDebug() << file.errorString();//文件打开错误显示错误信息
//    QByteArray arry=file.readAll();//读取文件
//    file.close();
    qDebug() << "打开文件：" << fileName;
    historyFile->setText(fileName);
//    int length=arry.size();//计算长度
//    qDebug() << length;
    historyView->LoadingData(fileName);     // 更新表格数据
//    historyDetail->LoadingData(fileName);
}

void RealTime::slotAverageValue(int val)
{
    m_ComData->SettingAverageTime = val;
    m_ComData->WriteData(AVERAGE_VALUE);
}

void RealTime::slotBatteryValue(int val)
{
    m_ComData->SettingBatteryCapacity = val;
    m_ComData->WriteData(BATTERY_CAPACITY_VALUE);
    bTotalCap->setText(QString::number(m_ComData->SettingBatteryCapacity));
}

void RealTime::onSettingBtn(void)
{
    send_CMD(0x08);     // 读取版本号和文件长度指令
//    tabWidget->setTabEnabled(2, false);
    frame_2_updata->setVisible(true);
    if(tabWidget->count() < 3)
        tabWidget->addTab(frame_2_updata, "设置");
    tabWidget->setCurrentIndex(2);
}

void RealTime::slotSendVerifyCmd(void)
{
//     qDebug() << "SendVerifyCount = " << SendVerifyCount << QTime::currentTime();
    send_CMD(0x20);     // 读取各个档位的校验值
    SendVerifyCount++;
    if(SendVerifyCount >= 10)
    {
        SendVerifyCount = 0;
        SendVerifyCmd->stop();
    }


}

void RealTime::slotFixCurrentScale(int val)
{
//    qDebug() << "val = " << val;
    double fixArr[9] = {0, 3000, 1000, 100, 10, 1, 0.1, 0.01, 0.001};
    try {
        fixCurrentValue = fixArr[val];
        updateChart();      // 初始化显示表格
    } catch (...) {

    }

}

void RealTime::slotQBoxTip(QString str)
{
    QMessageBox::critical(this, "提示", str);
}
