#include "averagesubframe.h"

AverageSubFrame::AverageSubFrame(QFrame *parent) : QFrame(parent)
{
    this->setObjectName("FrameQss");
    this->setStyleSheet("QFrame#FrameQss {border:1px solid black; background-color: rgb(96, 96, 96, 50);\
                            border-top-left-radius:4px;         \
                            border-top-right-radius:4px;        \
                            border-bottom-left-radius:4px;      \
                            border-bottom-right-radius:4px}");
}
