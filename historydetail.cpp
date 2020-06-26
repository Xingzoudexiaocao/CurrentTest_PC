#include "historydetail.h"

HistoryDetail::HistoryDetail(QWidget *parent) : QWidget(parent)
{

}

HistoryDetail::~HistoryDetail()
{

}

void HistoryDetail::ReceiveTest(void)
{
    qDebug() << "接收到信号！";
}
