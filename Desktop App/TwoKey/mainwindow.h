#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "usb_communicator.h"

#include <QMainWindow>
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
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    USB_communicator *usb_comm;
};

#endif // MAINWINDOW_H
