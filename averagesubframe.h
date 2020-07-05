#ifndef AVERAGESUBFRAME_H
#define AVERAGESUBFRAME_H

#include <QObject>
#include <QFrame>
#include <QRadioButton>
#include <QLabel>
#include <QDebug>
#include <QDateTime>

class AverageSubFrame : public QFrame
{
    Q_OBJECT
public:
    explicit AverageSubFrame(QFrame *parent = nullptr);

    qint8 getKeyValue(void);
    void setKeyValue(qint8 val);

    qint8 getCurVolFlag(void);
    void setCurVolFlag(qint8 val);

    void initFrameDisplay(void);
private:
    qint8 CurVolFlag;       // 电流电压标志位：1电流；2电压；
    qint8 keyValue;         // T1/T2标志位：1=T1；2=T2；
    QRadioButton *T1_Text;
    QRadioButton *T2_Text;
    QLabel *T2_T1_Label;
    QLabel *AverageLabel;

signals:

private slots:

    void slotButtonT1Check(bool);
    void slotButtonT2Check(bool);

public slots:
    void slotUpdateT1AndT2(qint8, qint64);
    void slotUpdateAverage(qint64, double);
};

#endif // AVERAGESUBFRAME_H
