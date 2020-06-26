#ifndef HISTORYDETAIL_H
#define HISTORYDETAIL_H

#include <QObject>
#include <QWidget>
#include "comdata.h"
#include <QtDebug>

class HistoryDetail : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryDetail(QWidget *parent = nullptr);
    ~HistoryDetail();

signals:

public slots:
    void ReceiveTest(void);
};

#endif // HISTORYDETAIL_H
