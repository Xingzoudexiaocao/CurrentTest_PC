#ifndef ABOUT_H
#define ABOUT_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>

#include "comdata.h"

class About : public QWidget
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = nullptr);
    ~About();

signals:

public slots:

private Q_SLOTS:
    void closeEvent ( QCloseEvent*);
    void onLineBuy(QString);
    void close(QString);
};

#endif // ABOUT_H
