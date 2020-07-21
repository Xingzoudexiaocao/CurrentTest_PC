#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent, ComData *comD, USB_HID *hid) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "MainWindow构造函数。";
    m_ComData = comD;
    m_UsbHid = hid;
    m_Debug = nullptr;
//    QTextCodec *codec = QTextCodec::codecForName("GBK");//情况1
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
//    QTextCodec::setCodecForTr(codec);
//    QTextCodec::setCodecForLocale(codec);
//    QTextCodec::setCodecForCStrings(codec);

//    resize(1280, 800);
//    setMinimumSize(1000, 700);
    setFixedSize(m_ComData->gUiSize->width(), m_ComData->gUiSize->height() - 60);            // 1100, 885        // 设定之后不能拉伸
//    setGeometry(100, 50, 1680, 1250);      // 设定窗口显示坐标           100, 50, 1680, 950
//    setGeometry(800, 900, 1100, 885);      // 设定窗口显示坐标
    setWindowTitle(productName);
    QIcon icon;
    #if (MCU_TYPE == iSCAN_STM32)
        icon.addFile(QStringLiteral(":/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
    #elif  (MCU_TYPE == iSCAN_ARTERY)
        icon.addFile(QStringLiteral(":/logo-Artery.png"), QSize(), QIcon::Normal, QIcon::Off);
    #elif  (MCU_TYPE == iSCAN_INTERNAL)
        icon.addFile(QStringLiteral(":/logo_2.png"), QSize(), QIcon::Normal, QIcon::Off);
    #endif
    setWindowIcon(icon);
//    setWindowIcon(QIcon(":/logo.png"));//这个路径是右键选择上图中图片，复制到路径。
    this->grabKeyboard();       // 捕获键盘输入

    demo = new RealTime(this, m_ComData, m_UsbHid);
//    demo->setStyleSheet("* {font-family:arial;font-size:15px}");
//    demo->setGeometry(8, 20, 990, 850);
//    QGridLayout *MainLayout = new QGridLayout(this);
//    MainLayout->addWidget(demo);
    demo->show();

//#ifdef  isDebuf
//    OpenDebugWindow();  // 默认开启
//#endif

    // 初始化接收缓存
    RecIndex = 0;
    for (unsigned long long i = 0; i < 1024; i++) {
        RecBuffer[i] = 0;
    }

}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow析构函数。";
    delete ui;
    delete m_ComData;
    delete m_UsbHid;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
     qDebug() << "frameGeometry().size() = " << frameGeometry().size();
//     this->resize(frameGeometry().size()); //是采用size()还是frameGeometry.size()根据自己的需求。
}

void MainWindow::colseDebug()
{
//    qDebug() << "Debug关闭。";
    m_Debug = nullptr;
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_P) {
        qDebug() << "Ctrl+p 按键按下。";
#ifdef  isDebuf
    OpenDebugWindow();
#endif
        return;
    }
    QWidget::keyPressEvent(ev);
}
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    if(ev->modifiers() == Qt::ControlModifier && ev->key() == Qt::Key_P) {
        qDebug() << "Ctrl+p 按键松开。";
        return;
    }
    QWidget::keyReleaseEvent(ev);
}

void MainWindow::OpenDebugWindow()
{
    if(m_Debug == nullptr)
    {
        m_Debug = new DebugWatch(nullptr, m_ComData, m_UsbHid);
//        demo->setStyleSheet("* {font-family:arial;font-size:15px}");
//        demo->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // 置顶，最小化，关闭
        m_Debug->setGeometry(0, 0, 400, 600);
        m_Debug->show();
        connect(demo->m_UsbReceiveThread,SIGNAL(send_Level_Num(int)),m_Debug, SLOT(receive_Level_Num(int)));
        connect(demo->m_UsbReceiveThread,SIGNAL(get_Verify_Value()),m_Debug, SLOT(receive_Verify_Value()));
        connect(m_Debug,SIGNAL(destroyed()),this, SLOT(colseDebug()));
    }
}


