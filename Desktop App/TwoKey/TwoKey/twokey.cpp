#include "twokey.h"
#include "ui_twokey.h"

#include <QThread>

TwoKey::TwoKey(QWidget *parent, USB_communicator *usb_comm) :
    QWidget(parent)
    , ui(new Ui::TwoKey)
{
    ui->setupUi(this);

    this->usb_comm = usb_comm;
    connect(this->usb_comm->usb_notif, SIGNAL(tokenStatusChanged()), this, SLOT(changeStatus()));

    this->usb_comm->usb_notif->checkDeviceID();
    this->usb_comm->checkForToken();

    this->browserExtensionThread = new QThread();
    this->browserExtensionComm = new BrowserExtensionCommunicator();

    browserExtensionComm->moveToThread(browserExtensionThread);

    connect(browserExtensionThread, SIGNAL(started()), browserExtensionComm, SLOT(startServer()));
    connect(browserExtensionThread, SIGNAL(finished()), browserExtensionComm, SLOT(stopServer()));
    browserExtensionThread->start();


    ui->twokey_stackedwidget->setCurrentIndex(0);
    ui->manager_save_button->setVisible(false);
    ui->manager_generate_button->setVisible(false);
    ui->manager_logout_button->setVisible(false);
    ui->manager_logout_button->setEnabled(false);

    returnShortcut = new QShortcut(QKeySequence("Return"), ui->login_page);
    QObject::connect(returnShortcut, SIGNAL(activated()), ui->login_button, SLOT(click()));
}

TwoKey::~TwoKey()
{
    delete usb_comm;
    delete returnShortcut;
    delete ui;
}

bool TwoKey::backend_login(QString email, QString password)
{
    QNetworkRequest request(QUrl("https://64.227.127.192/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("email", email);
    json.insert("password", password);

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
    tokenChallengeResponse = usb_comm->readFromToken();

    qDebug() << "Message: " + challenge;
    qDebug() << "Response:" + tokenChallengeResponse;

    if (backend_2fa())
    {
        return true;
    }
    else return false;
}

bool TwoKey::backend_register(QString firstName, QString lastName, QString email, QString username, QString password, QString serial)
{
    QNetworkRequest request(QUrl("https://64.227.127.192/reg"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("first_name", firstName);
    json.insert("last_name", lastName);
    json.insert("email", email);
    json.insert("username", username);
    json.insert("password", password);
    json.insert("serial", serial);

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

    if (jsonResponse.object()["Message"].toString().contains("Done"))
    {
            return true;
    }
    else return false;
}

bool TwoKey::backend_2fa()
{
    QNetworkRequest request(QUrl("https://64.227.127.192/2fa"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", jwt);
    json.insert("challenge", QString(tokenChallengeResponse.toUtf8().toBase64()));
    qDebug() << "Encrypted Challenge: " << tokenChallengeResponse;
    qDebug() << "Base64 Challenge: " << QString(tokenChallengeResponse.toUtf8().toBase64());
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

    for (auto a :jsonResponse.object().keys())
    qDebug() << a << jsonResponse.object()[a].toString();

    if (jsonResponse.object()["Message"].toString() == "Successful Login!!")
    {
        qDebug() << "\nResponse(New JWT): " <<  this->jwt;
        return true;
    }
    else
    {
        qDebug() << "\nResponse: " <<  jsonResponse.object()["Message"].toString();
        return false;
    }
}

bool TwoKey::backend_logout()
{
    this->jwt.clear();
    return true;
}

/*
 _   _    ___     _____ ____    _  _____ ___ ___  _   _
| \ | |  / \ \   / |_ _/ ___|  / \|_   _|_ _/ _ \| \ | |
|  \| | / _ \ \ / / | | |  _  / _ \ | |  | | | | |  \| |
| |\  |/ ___ \ V /  | | |_| |/ ___ \| |  | | |_| | |\  |
|_| \_/_/   \_\_/  |___\____/_/   \_|_| |___\___/|_| \_|

*/

//                FIRST page LOGIN

void TwoKey::on_login_button_clicked()
{
    if (backend_login(ui->login_email->text(),
                      ui->login_password->text()))
    {
        ui->twokey_stackedwidget->setCurrentIndex(2); // LOGIN BUTTON
        ui->login_email->clear();
        ui->login_password->clear();
    }
    else
    {
        ui->login_password->clear();
    }
}

void TwoKey::on_createaccount_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(1); // CREATE ACCOUNT BUTTON
}

//                SECOND page SIGN UP

void TwoKey::on_register_button_clicked()
{
    if (backend_register(ui->register_firstname->text(),
                         ui->register_lastname->text(),
                         ui->register_email->text(),
                         ui->register_username->text(),
                         ui->register_password->text(),
                         ui->register_serial->text()))
    {
        ui->twokey_stackedwidget->setCurrentIndex(0); // CREATE ACCOUNT BUTTON
        ui->register_firstname->clear();
        ui->register_lastname->clear();
        ui->register_email->clear();
        ui->register_username->clear();
        ui->register_password->clear();
        ui->register_serial->clear();
    }
}

void TwoKey::on_register_back_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(0); // LOGIN BUTTON
}

//                THIRD page MANAGER

void TwoKey::on_manager_addaccount_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(3); // ADD ACCOUNT BUTTON
}

void TwoKey::on_manager_logout_button_clicked()
{
    backend_logout();
    ui->twokey_stackedwidget->setCurrentIndex(0);
}

//                FORTH page ADD ACCOUNT

void TwoKey::on_addaccount_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(2); // ADD ACCOUNT BUTTON
}

void TwoKey::on_addaccount_cancel_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(2); // CANCEL BUTTON
}

/*
 __  __    _    _   _    _    ____ _____ ____     _____ ____ ___ _____    ___ _   _ _____ ___     ____  _   _ _____ _____ ___  _   _ ____
|  \/  |  / \  | \ | |  / \  / ___| ____|  _ \   | ____|  _ |_ _|_   _|  |_ _| \ | |  ___/ _ \   | __ )| | | |_   _|_   _/ _ \| \ | / ___|
| |\/| | / _ \ |  \| | / _ \| |  _|  _| | |_) |  |  _| | | | | |  | |     | ||  \| | |_ | | | |  |  _ \| | | | | |   | || | | |  \| \___ \
| |  | |/ ___ \| |\  |/ ___ | |_| | |___|  _ <   | |___| |_| | |  | |     | || |\  |  _|| |_| |  | |_) | |_| | | |   | || |_| | |\  |___) |
|_|  |_/_/   \_|_| \_/_/   \_\____|_____|_| \_\  |_____|____|___| |_|    |___|_| \_|_|   \___/   |____/ \___/  |_|   |_| \___/|_| \_|____/

*/

//                ONLY    THIRD page MANAGER

void TwoKey::on_manager_edit_button_clicked()    //    EDIT INFO BUTTON
{
    ui->manager_edit_button->setEnabled(false);
    ui->manager_edit_button->setVisible(false);
    ui->manager_website->setReadOnly(false);
    ui->manager_username->setReadOnly(false);
    ui->manager_password->setReadOnly(false);
    ui->manager_generate_button->setVisible(true);
    ui->manager_generate_button->setEnabled(true);
    ui->manager_save_button->setVisible(true);
    ui->manager_save_button->setEnabled(true);
}

void TwoKey::on_manager_save_button_clicked()    //    SAVE INFO BUTTON
{
    ui->manager_edit_button->setEnabled(true);
    ui->manager_edit_button->setVisible(true);
    ui->manager_website->setReadOnly(true);
    ui->manager_username->setReadOnly(true);
    ui->manager_password->setReadOnly(true);
    ui->manager_generate_button->setVisible(false);
    ui->manager_generate_button->setEnabled(false);
    ui->manager_save_button->setEnabled(false);
    ui->manager_save_button->setVisible(false);
}

/*
 ____   _    ____ ______        _____  ____  ____     ____   _____  _    ____  _   _ _____ _____ ___  _   _ ____
|  _ \ / \  / ___/ ___\ \      / / _ \|  _ \|  _ \   | __ ) / _ \ \/ /  | __ )| | | |_   _|_   _/ _ \| \ | / ___|
| |_) / _ \ \___ \___ \\ \ /\ / | | | | |_) | | | |  |  _ \| | | \  /   |  _ \| | | | | |   | || | | |  \| \___ \
|  __/ ___ \ ___) ___) |\ V  V /| |_| |  _ <| |_| |  | |_) | |_| /  \   | |_) | |_| | | |   | || |_| | |\  |___) |
|_| /_/   \_|____|____/  \_/\_/  \___/|_| \_|____/   |____/ \___/_/\_\  |____/ \___/  |_|   |_| \___/|_| \_|____/

*/

//                FIRST page LOGIN

void TwoKey::on_login_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->login_password->echoMode() == QLineEdit::Password){
        ui->login_password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->login_password->setEchoMode(QLineEdit::Password);
    }
}


//                SECOND page SIGN UP

void TwoKey::on_register_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->register_password->echoMode() == QLineEdit::Password){
        ui->register_password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->register_password->setEchoMode(QLineEdit::Password);
    }
}

//                THIRD page MANAGER

void TwoKey::on_manager_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->manager_password->echoMode() == QLineEdit::Password){
        ui->manager_password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->manager_password->setEchoMode(QLineEdit::Password);
    }
}

void TwoKey::on_manager_copypassword_button_clicked()    //    COPY PASSWORD TO CLIPBOARD
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->manager_password->text());
}

void TwoKey::on_manager_generate_button_clicked()    //    GENERATE PASSWORD
{
    ui->manager_password->setText("PASSWORDMANAGER");
}

//                FORTH page ADD ACCOUNT


void TwoKey::on_addaccount_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->addaccount_password->echoMode() == QLineEdit::Password){
        ui->addaccount_password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->addaccount_password->setEchoMode(QLineEdit::Password);
    }
}

void TwoKey::on_addaccount_generate_button_clicked()    //    GENERATE PASSWORD
{
    ui->manager_password->setText("PASSWORDMANAGER");
}

void TwoKey::changeStatus()
{
    this->usb_comm->checkForToken();
    if (usb_comm->getTokenStatus())
    {
        //Set to green
        ui->statusLabel->setPixmap(QPixmap("://Icons/StatusIcons/greenStatusIconCircle.png").scaled(ui->statusLabel->maximumWidth(),
                                                                                     ui->statusLabel->maximumHeight(),
                                                                                     Qt::KeepAspectRatio));
        this->ui->statusLabel->setToolTip("TwoKey's token is connected.");
    }
    else
    {
        //Set to red
        ui->statusLabel->setPixmap(QPixmap("://Icons/StatusIcons/redStatusIconCircle.png").scaled(ui->statusLabel->maximumWidth(),
                                                                                     ui->statusLabel->maximumHeight(),
                                                                                     Qt::KeepAspectRatio));
        this->ui->statusLabel->setToolTip("TwoKey's token is disconnected.");
        backend_logout();
        ui->twokey_stackedwidget->setCurrentIndex(0);

    }
}

void TwoKey::on_twokey_stackedwidget_currentChanged(int arg1)
{
    if(arg1 == 2){
        ui->manager_logout_button->setVisible(true);
        ui->manager_logout_button->setEnabled(true);
    }
    else{
        ui->manager_logout_button->setVisible(false);
        ui->manager_logout_button->setEnabled(false);
    }
}
