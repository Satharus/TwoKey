#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, USBEventHandler *usb_notif) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->usb_notif = usb_notif;

    connect(this->usb_notif, SIGNAL(tokenStatusChanged()), this, SLOT(changeStatus()));
    this->usb_notif->checkForToken();
}

MainWindow::~MainWindow()
{
    delete usb_notif;
    delete ui;
}

void MainWindow::changeStatus()
{
    if (usb_notif->tokenConnected)
    {
        //Set to green
        this->ui->statusLabel->setStyleSheet("background: rgb(65,157,60)");
    }
    else
    {
        //Set to red
        this->ui->statusLabel->setStyleSheet("background: rgb(157,60,60)");
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->usb_notif->checkForToken();
}
