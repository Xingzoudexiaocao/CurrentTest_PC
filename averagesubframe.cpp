#include "averagesubframe.h"

AverageSubFrame::AverageSubFrame(QFrame *parent) : QFrame(parent)
{
    // size(200, 100)
    this->setObjectName("FrameQss");
    this->setStyleSheet("QFrame#FrameQss {border:1px solid black; background-color: rgb(96, 96, 96, 50);\
                            border-top-left-radius:4px;         \
                            border-top-right-radius:4px;        \
                            border-bottom-left-radius:4px;      \
                            border-bottom-right-radius:4px}");
    keyValue = 1;
    T1_Text = new QRadioButton(this);
    T1_Text->setText("T1 = 00:00:00.000");
    T1_Text->setGeometry(5, 0, 200, 25);
    T1_Text->setStyleSheet("QRadioButton {color:red;}");
    T1_Text->setChecked(true);
    connect(T1_Text, SIGNAL(clicked(bool)), this, SLOT(slotButtonT1Check(bool)));
//    connect(T1_Text, &QAbstractButton::clicked, this, &AverageSubFrame::slotButtonCheck());
    T2_Text = new QRadioButton(this);
    T2_Text->setText("T2 = 00:00:00.000");
    T2_Text->setGeometry(5, 25, 200, 25);
    T2_Text->setStyleSheet("QRadioButton {color:red;}");
    connect(T2_Text, SIGNAL(clicked(bool)), this, SLOT(slotButtonT2Check(bool)));
    T2_T1_Label = new QLabel(this);
    T2_T1_Label->setText("T2-T1 = 00:00:00.000");
    T2_T1_Label->setGeometry(5, 50, 200, 25);
    T2_T1_Label->setStyleSheet("QLabel {color:red;}");
    AverageLabel = new QLabel(this);
    AverageLabel->setText("平均值 = --");
    AverageLabel->setGeometry(5, 75, 200, 25);
    AverageLabel->setStyleSheet("QLabel {color:red;}");
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
    AverageLabel->setText("平均值 = --");
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

void AverageSubFrame::slotUpdateAverage(qint64 DifferT, double avergeVal)
{
    T2_T1_Label->setText("T2-T1 = " + QDateTime::fromMSecsSinceEpoch(DifferT).toString("00:mm:ss.zzz"));
    if(CurVolFlag == 1)
    {
        if(avergeVal < 1)
            AverageLabel->setText("平均值 = " + QString::number(avergeVal * 1000, 'f', 3) + "uA");
        else
            AverageLabel->setText("平均值 = " + QString::number(avergeVal, 'f', 3) + "mA");
    }
    else if(CurVolFlag == 2)
    {
        AverageLabel->setText("平均值 = " + QString::number(avergeVal, 'f', 3) + "V");
    }
}
