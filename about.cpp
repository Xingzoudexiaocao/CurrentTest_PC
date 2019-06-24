#include "about.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>

using namespace std;

About::About(QWidget *parent) : QMainWindow(parent)
{
    qDebug() << "About构造函数。";
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint); // 置顶，最小化，关闭
    setWindowTitle("关于   " + compangyName);

    QPixmap pixmap(":/logo.png");
    QLabel *label = new QLabel(this);
    label->setGeometry(8, 8, 200, 200);
//    label->setStyleSheet("background-color:black");
    label->setPixmap(pixmap.scaled(label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    QFrame *frame = new QFrame(this);
    frame->setGeometry(216, 8, 380, 240);
    frame->setFrameShape(QFrame::NoFrame);

//    QFont    font ( "微软雅黑",  20,   75);
    QDateTime lastTime = QDateTime::currentDateTime();
    QLabel *label_1 = new QLabel(frame);
    label_1->setGeometry(0, 0, 380, 80);
    label_1->setStyleSheet("QLabel {font-family:微软雅黑; text-align:left; padding:2px; font-size:24px;}");
//    label_1->setFont(font);
    label_1->setText(productName + " 19.1000.0.1 \n© " + QString::number(lastTime.date().year()) + " " + compangyName + "。保留所有权利。");
    QLabel *label_2 = new QLabel(frame);
    label_2->setGeometry(0, 80, 380, 80);
    label_2->setStyleSheet("QLabel {font-family:微软雅黑; text-align:left; padding:2px; font-size:20px;}");
//    label_2->setFont(font);
    label_2->setText("联系人：***\n联系电话：188********");

    QLabel *label_3 = new QLabel(frame);
    label_3->setGeometry(200, 180, 100, 40);
    label_3->setStyleSheet("QLabel {font-family:微软雅黑; text-align:left; padding:2px; font-size:20px;}");
//    label_3->setFont(font);
    label_3->setText("<a href=\"https://www.taobao.com/\">在线购买");
    connect(label_3,SIGNAL(linkActivated (QString)),this,SLOT(onLineBuy(QString)));   // const QString&

    QLabel *label_4 = new QLabel(frame);
    label_4->setGeometry(320, 180, 60, 40);
    label_4->setStyleSheet("QLabel {font-family:微软雅黑; text-align:left; padding:2px; font-size:20px;}");
//    label_4->setFont(font);
    label_4->setText("<a href=\"https://www.taobao.com/\">关闭");
    connect(label_4,SIGNAL(linkActivated (QString)),this,SLOT(close (QString)));   // const QString&

}

About::~About()
{
    qDebug() << "About析构函数。";

}

void About::closeEvent ( QCloseEvent * e )
{
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void About::close(QString str)
{
//    qDebug() << "关闭" << str;
//    this->setAttribute(Qt::WA_QuitOnClose);
//    this->setAttribute(Qt::WA_DeleteOnClose);

    this->~About();
//    this->destroy(true, true);
}

void About::onLineBuy(QString str)
{
    QDesktopServices::openUrl(QUrl(str));
    //若是文件路径，则需使用下面的打开方式，具体可参见QUrl帮助文档
    //    QDesktopServices::openUrl(QUrl("file:///" + url, QUrl::TolerantMode));

}
