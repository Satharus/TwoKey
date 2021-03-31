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
// https://amin-ahmadi.com/2016/01/17/how-to-send-and-receive-json-requests-in-qt/
// https://stackoverflow.com/questions/13302236/qt-simple-post-request
QString test;
void MainWindow::postRequest()
{
    QNetworkRequest request(QUrl("https://64.227.127.192/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("email", "oracle@gmail.com");
    json.insert("password", "test1234");

    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());
    reply->ignoreSslErrors();
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

    QString challenge = jsonResponse.object()["challenge"].toString();
    this->jwt = jsonResponse.object()["Access-token"].toString();
    qDebug() << "Recieved Challenge: " << challenge;

    usb_comm->writeToToken(challenge.toStdString().c_str());
    test = usb_comm->readFromToken();

    this->ui->sentValue->setText("Message: " + challenge);
    this->ui->response->setText("Response:" + test);

    Danya2FA();
}


void MainWindow::Danya2FA()
{
    QNetworkRequest request(QUrl("https://64.227.127.192/2fa"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", jwt);
    json.insert("challenge", QString(test.toUtf8().toBase64()));
    qDebug() << "Encrypted Challenge: " << test;
    qDebug() << "Base64 Challenge: " << QString(test.toUtf8().toBase64());
    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    reply->ignoreSslErrors();

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    this->jwt = jsonResponse.object()["Access-token"].toString();
    if (jsonResponse.object()["Message"].toString() == "Successful Login!!")
        qDebug() << "\nResponse(New JWT): " <<  this->jwt;
    else
        qDebug() << "\nResponse: " <<  jsonResponse.object()["Message"].toString();
}



void MainWindow::registerDanya()
{
    QNetworkRequest request(QUrl("https://64.227.127.192/reg"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("first_name", "Ahmed");
    json.insert("last_name", "Mahmoud");
    json.insert("email", "oracle@gmail.com");
    json.insert("username", "oracle");
    json.insert("password", "test1234");
    json.insert("physical_id", "testtoken0123456");

    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    reply->ignoreSslErrors();

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

    qDebug() << "\nResponse: " <<  jsonResponse;

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

void MainWindow::on_pushButton_clicked()
{
    postRequest();
}

void MainWindow::on_pushButton_2_clicked()
{
    registerDanya();
}
