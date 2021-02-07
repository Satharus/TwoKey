#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, USB_communicator *usb_comm) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->usb_comm = usb_comm;
    this->stdinNotifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Type::Read, this);

    connect(this->usb_comm->usb_notif, SIGNAL(tokenStatusChanged()), this, SLOT(changeStatus()));
    connect(this->stdinNotifier, SIGNAL(activated(int)), this, SLOT(readFromStdin()));

    this->usb_comm->usb_notif->checkDeviceID();
    this->usb_comm->checkForToken();
}

MainWindow::~MainWindow()
{
    delete stdinNotifier;
    delete usb_comm;
    delete ui;
}

void MainWindow::changeStatus()
{
    this->usb_comm->checkForToken();
    if (usb_comm->getTokenStatus())
    {
        //Set to green
        this->ui->statusLabel->setStyleSheet("background: rgb(65,157,60)");
        this->ui->sentValue->setText("Message:  " + this->usb_comm->getLastMessage());
        this->ui->response->setText("Response: " + this->usb_comm->getLastResponse());
    }
    else
    {
        //Set to red
        this->ui->statusLabel->setStyleSheet("background: rgb(157,60,60)");
        this->ui->sentValue->setText("Message:  ");
        this->ui->response->setText("Response: ");
    }
}

void MainWindow::on_refreshButton_clicked()
{
    this->usb_comm->checkForToken();
    this->changeStatus();
}

void MainWindow::on_writeButton_clicked()
{
    this->usb_comm->writeToToken(this->ui->writeBox->text().toStdString().c_str());
    this->ui->sentValue->setText("Message:  " + this->ui->writeBox->text());
    this->ui->writeBox->clear();
}

void MainWindow::on_readButton_clicked()
{
    this->ui->response->setText("Response: " + this->usb_comm->readFromToken());
}

QString MainWindow::readFromStdin()
{
    std::string stdinMessage;
    std::cin >> stdinMessage;
    QString retMessage = QString::fromStdString(stdinMessage);


    this->ui->browserExtensionLabel->setText("Extension Message: " + retMessage);

    return retMessage;
}
