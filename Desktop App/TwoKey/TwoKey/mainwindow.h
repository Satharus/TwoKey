#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "usb_communicator.h"

#include <QtNetwork>

#include <QSocketNotifier>
#include <QMainWindow>
#include <iostream>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, USB_communicator *usb_comm = nullptr);
    ~MainWindow();

    void postRequest();
    void registerDanya();
    void Danya2FA();

private slots:
    void changeStatus();
    void on_refreshButton_clicked();

    void on_writeButton_clicked();

    void on_readButton_clicked();
    QString readFromStdin();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    USB_communicator *usb_comm;
    QSocketNotifier *stdinNotifier;
    QString jwt;
};

#endif // MAINWINDOW_H
