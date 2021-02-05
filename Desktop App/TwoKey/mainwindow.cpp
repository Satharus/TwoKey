#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, USB_communicator *usb_comm) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->usb_comm = usb_comm;
    connect(this->usb_comm->usb_notif, SIGNAL(tokenStatusChanged()), this, SLOT(changeStatus()));
    this->usb_comm->usb_notif->checkDeviceID();
    this->usb_comm->checkForToken();
}

MainWindow::~MainWindow()
{
    delete usb_comm;
    delete ui;
}

void MainWindow::changeStatus()
{
    if (usb_comm->getTokenStatus())
    {
        //Set to green
        this->ui->statusLabel->setStyleSheet("background: rgb(65,157,60)");
        this->ui->sentValue->setText("Message:  " + this->usb_comm->getMessage());
        this->ui->response->setText("Response: " + this->usb_comm->getResponse());
    }
    else
    {
        //Set to red
        this->ui->statusLabel->setStyleSheet("background: rgb(157,60,60)");
        this->ui->sentValue->setText("Message:  ");
        this->ui->response->setText("Response: ");
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->usb_comm->checkForToken();
    this->changeStatus();
}
