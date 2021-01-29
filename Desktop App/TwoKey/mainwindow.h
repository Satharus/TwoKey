#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "usb_event_handler.h"

#include <QMainWindow>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, USBEventHandler *usb_notif = nullptr);
    ~MainWindow();

private slots:
    void changeStatus();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    //Pointer to the USB handler, to get updates from it
    USBEventHandler *usb_notif;
};

#endif // MAINWINDOW_H
