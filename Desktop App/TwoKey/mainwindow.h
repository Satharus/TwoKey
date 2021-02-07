#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "usb_communicator.h"

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

private slots:
    void changeStatus();
    void on_refreshButton_clicked();

    void on_writeButton_clicked();

    void on_readButton_clicked();
    QString readFromStdin();

private:
    Ui::MainWindow *ui;
    USB_communicator *usb_comm;
    QSocketNotifier *stdinNotifier;
};

#endif // MAINWINDOW_H
