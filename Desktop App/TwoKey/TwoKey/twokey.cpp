#include "twokey.h"
#include "ui_twokey.h"

TwoKey::TwoKey(QWidget *parent, USB_communicator *usb_comm) :
    QWidget(parent)
    , ui(new Ui::TwoKey)
{
    ui->setupUi(this);

    this->usb_comm = usb_comm;
    connect(this->usb_comm->usb_notif, SIGNAL(tokenStatusChanged()), this, SLOT(changeStatus()));

    this->usb_comm->usb_notif->checkDeviceID();
    this->usb_comm->checkForToken();


    ui->twokey_stackedwidget->setCurrentIndex(0);
    ui->manager_save_button->setVisible(false);
    ui->manager_generate_button->setVisible(false);
}

TwoKey::~TwoKey()
{
    delete ui;
}

// https://amin-ahmadi.com/2016/01/17/how-to-send-and-receive-json-requests-in-qt/
// https://stackoverflow.com/questions/13302236/qt-simple-post-request
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
    if (backend_login(ui->login_username->text(),
                      ui->login_password->text()))
    {
        ui->twokey_stackedwidget->setCurrentIndex(2); // LOGIN BUTTON
    }
}

void TwoKey::on_createaccount_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(1); // CREATE ACCOUNT BUTTON
}

//                SECOND page SIGN UP

void TwoKey::on_signup_button_clicked()
{
    if (backend_register(ui->signup_firstname->text(),
                         ui->signup_lastname->text(),
                         ui->signup_email->text(),
                         ui->signup_username->text(),
                         ui->signup_password->text(),
                         ui->signup_serial->text()))
    {
        ui->twokey_stackedwidget->setCurrentIndex(0); // CREATE ACCOUNT BUTTON
    }
}

void TwoKey::on_signup_login_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(0); // LOGIN BUTTON
}

//                THIRD page MANAGER

void TwoKey::on_manager_addaccount_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(3); // ADD ACCOUNT BUTTON
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

void TwoKey::on_signup_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->signup_password->echoMode() == QLineEdit::Password){
        ui->signup_password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->signup_password->setEchoMode(QLineEdit::Password);
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
