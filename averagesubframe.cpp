#include "averagesubframe.h"

AverageSubFrame::AverageSubFrame(QFrame *parent) : QFrame(parent)
{
    // size(200, 100)

    keyValue = 1;

    int fontId = QFontDatabase::addApplicationFont(":/ZhuoJianGanLanJianTi.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    font.setFamily(fontFamilies.at(0));
    QString qssRadio = "QRadioButton {font-family:arial; border-radius: 2px; text-align:left; font-size:20px;font-weight:bold;} \
            QRadioButton::indicator:checked {  image: url(:/checkbox_checked.png); } \
            QRadioButton::indicator:unchecked {  image: url(:/checkbox_unchecked.png);}\
            QRadioButton::indicator { width: 30px; height: 30px;}";
    QString qssLabel_1 = "QLabel {font-family:arial;  text-align:left; padding:2px; font-size:20px;font-weight:bold;}";
    QString qssLabel_2 = "QLabel {  text-align:left; padding:2px; font-size:16px;font-weight:bold;}";         // color:#DC143C;

    QString qssThis = "QFrame#FrameQss {border:1px solid black; \
            border-top-left-radius:4px;         \
            border-top-right-radius:4px;        \
            border-bottom-left-radius:4px;      \
            border-bottom-right-radius:4px}";

    #if (MCU_TYPE == iSCAN_STM32)
        qssThis.append("QFrame#FrameQss{background-color: rgb(240, 240, 240, 200);}");
        qssRadio.append("QRadioButton{color:#0000FF;}");
        qssLabel_1.append("QLabel{color:#0000FF;}");
        qssLabel_2.append("QLabel{color:#0000FF;}");
    #elif  (MCU_TYPE == iSCAN_ARTERY)
        qssThis.append("QFrame#FrameQss{background-color: rgb(70, 70, 70, 200);}");
        qssRadio.append("QRadioButton{color:#AAD0FF;}");
        qssLabel_1.append("QLabel{color:#AAD0FF;}");
        qssLabel_2.append("QLabel{color:#AAD0FF;}");
    #elif  (MCU_TYPE == iSCAN_INTERNAL)
        qssThis.append("QFrame#FrameQss{background-color: rgb(240, 240, 240, 200);}");
        qssRadio.append("QRadioButton{color:#0000FF;}");
        qssLabel_1.append("QLabel{color:#0000FF;}");
        qssLabel_2.append("QLabel{color:#0000FF;}");
    #endif


    this->setObjectName("FrameQss");
    this->setStyleSheet(qssThis);

    T1_Text = new QRadioButton(this);
    T1_Text->setText("T1 = 00:00:00.000");
    T1_Text->setGeometry(5, 5, 220, 30);
    T1_Text->setStyleSheet(qssRadio);      // QRadioButton {color:red;}
//    T1_Text->setFont(font);
    T1_Text->setChecked(true);
    connect(T1_Text, SIGNAL(clicked(bool)), this, SLOT(slotButtonT1Check(bool)));
//    connect(T1_Text, &QAbstractButton::clicked, this, &AverageSubFrame::slotButtonCheck());
    T2_Text = new QRadioButton(this);
    T2_Text->setText("T2 = 00:00:00.000");
    T2_Text->setGeometry(5 + 220, 5, 220, 30);
    T2_Text->setStyleSheet(qssRadio);      // QRadioButton {color:red;}
//    T2_Text->setFont(font);
    connect(T2_Text, SIGNAL(clicked(bool)), this, SLOT(slotButtonT2Check(bool)));
    T2_T1_Label = new QLabel(this);
    T2_T1_Label->setText("T2-T1 = 00:00:00.000");
    T2_T1_Label->setGeometry(5 + 220 + 220, 5, 220, 30);
    T2_T1_Label->setStyleSheet(qssLabel_1);      // QRadioButton {color:red;}
//    T2_T1_Label->setFont(font);
    AverageLabel = new QLabel(this);
    AverageLabel->setText("电流平均值=--");
    AverageLabel->setGeometry(5, 30, 220, 25);
    AverageLabel->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    AverageLabel->setFont(font);
    MaxLabel = new QLabel(this);
    MaxLabel->setText("电流最大值=--");
    MaxLabel->setGeometry(5 + 220, 30, 220, 25);
    MaxLabel->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    MaxLabel->setFont(font);
    MinLabel = new QLabel(this);
    MinLabel->setText("电流最小值=--");
    MinLabel->setGeometry(5 + 220 + 220, 30, 220, 25);
    MinLabel->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    MinLabel->setFont(font);
    AverageLabel_2 = new QLabel(this);
    AverageLabel_2->setText("电压平均值=--");
    AverageLabel_2->setGeometry(5, 55, 220, 25);
    AverageLabel_2->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    AverageLabel_2->setFont(font);
    MaxLabel_2 = new QLabel(this);
    MaxLabel_2->setText("电压最大值=--");
    MaxLabel_2->setGeometry(5 + 220, 55, 220, 25);
    MaxLabel_2->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    MaxLabel_2->setFont(font);
    MinLabel_2 = new QLabel(this);
    MinLabel_2->setText("电压最小值=--");
    MinLabel_2->setGeometry(5 + 220 + 220, 55, 220, 25);
    MinLabel_2->setStyleSheet(qssLabel_2);      // QRadioButton {color:red;}
    MinLabel_2->setFont(font);

    subFrameClose = new QPushButton(this);      // QIcon(":/ExitA.png"),
    subFrameClose->setGeometry(665, 0, 35, 35);
//    subFrameClose->setStyleSheet("QPushButton{border-image: url(:/close_filled.png);}");
    subFrameClose->setObjectName("btnMenu_Close");
    IconHelper::Instance()->SetIcon(subFrameClose, QChar(0xf00d), 16);
//    subFrameClose->setFont(font);
    subFrameClose->setToolTip("点击关闭测量界面");
    connect(subFrameClose, &QAbstractButton::clicked, this, &AverageSubFrame::slotSubFrameClose);
}

qint8 AverageSubFrame::getKeyValue(void)
{
    return keyValue;
}
void AverageSubFrame::setKeyValue(qint8 val)
{
    keyValue = val;
}

qint8 AverageSubFrame::getCurVolFlag(void)
{
    return CurVolFlag;
}
void AverageSubFrame::setCurVolFlag(qint8 val)
{
    CurVolFlag = val;
}

void AverageSubFrame::initFrameDisplay(void)
{
    keyValue = 1;
    T1_Text->setChecked(true);
    T1_Text->setText("T1 = 00:00:00.000");
    T2_Text->setText("T2 = 00:00:00.000");
    T2_T1_Label->setText("T2-T1 = 00:00:00.000");
    AverageLabel->setText("电流平均值=--");
    MaxLabel->setText("电流最大值=--");
    MinLabel->setText("电流最小值=--");
    AverageLabel_2->setText("电压平均值=--");
    MaxLabel_2->setText("电压最大值=--");
    MinLabel_2->setText("电压最小值=--");
}

void AverageSubFrame::slotButtonT1Check(bool val)
{
//    qDebug() << "Click ButtonT1 = " << val;
    keyValue = 1;
}

void AverageSubFrame::slotButtonT2Check(bool val)
{
//    qDebug() << "Click ButtonT2 = " << val;
    keyValue = 2;
}

void AverageSubFrame::slotUpdateT1AndT2(qint8 T1OrT2, qint64 Time)
{
    if(T1OrT2 == 1)
        T1_Text->setText("T1 = " + QDateTime::fromMSecsSinceEpoch(Time).toString("hh:mm:ss.zzz"));
    else if(T1OrT2 == 2)
        T2_Text->setText("T2 = " + QDateTime::fromMSecsSinceEpoch(Time).toString("hh:mm:ss.zzz"));
}

void AverageSubFrame::slotUpdateCurAverage(qint64 DifferT, double avergeVal, double maxVal, double minVal)
{
    T2_T1_Label->setText("T2-T1 = " + QDateTime::fromMSecsSinceEpoch(DifferT).toString("00:mm:ss.zzz"));

    if(avergeVal < 1)
        AverageLabel->setText("电流平均值=" + QString::number(avergeVal * 1000, 'f', 3) + "uA");
    else
        AverageLabel->setText("电流平均值=" + QString::number(avergeVal, 'f', 3) + "mA");
    if(maxVal < 1)
        MaxLabel->setText("电流最大值=" + QString::number(maxVal * 1000, 'f', 3) + "uA");
    else
        MaxLabel->setText("电流最大值=" + QString::number(maxVal, 'f', 3) + "mA");
    if(minVal < 1)
        MinLabel->setText("电流最小值=" + QString::number(minVal * 1000, 'f', 3) + "uA");
    else
        MinLabel->setText("电流最小值=" + QString::number(minVal, 'f', 3) + "mA");

}

void AverageSubFrame::slotUpdateVolAverage(qint64 DifferT, double avergeVal, double maxVal, double minVal)
{
    T2_T1_Label->setText("T2-T1 = " + QDateTime::fromMSecsSinceEpoch(DifferT).toString("00:mm:ss.zzz"));
    AverageLabel_2->setText("电压平均值=" + QString::number(avergeVal, 'f', 3) + "V");
    MaxLabel_2->setText("电压最大值=" + QString::number(maxVal, 'f', 3) + "V");
    MinLabel_2->setText("电压最小值=" + QString::number(minVal, 'f', 3) + "V");
}

void AverageSubFrame::slotSubFrameClose(void)
{
    if(this->isVisible())
        this->setVisible(false);
}
