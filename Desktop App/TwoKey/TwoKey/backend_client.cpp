#include "backend_client.h"

BackendClient::BackendClient(USBCommunicator *usbComm) : QObject ()
{
    this->usbComm = usbComm;
}

int BackendClient::login(QString email, QString password)
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

    usbComm->writeToToken(challenge.toStdString().c_str());
    tokenChallengeResponse = usbComm->readFromToken();

    QByteArray response = QByteArray::fromHex(tokenChallengeResponse.toUtf8());

    qDebug() << "Message: " + challenge;
    qDebug() << "Response:" + response.toHex(',');


    int loginStatus = _2fa();
    return  loginStatus;
}



bool BackendClient::_register(QString firstName, QString lastName, QString email, QString username, QString password, QString serial)
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

    if (jsonResponse.object()["code"].toString() == "200")
    {
            return true;
    }
    else return false;
}

int BackendClient::_2fa()
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
    {
        qDebug() << a << jsonResponse.object()[a].toString();
    }

    if (jsonResponse.object()["code"].toString() == "200")
    {
        qDebug() << "\nResponse(New JWT): " <<  this->jwt;
        return loginStatus::SUCCESS;
    }
    else
    {
        for (auto a :jsonResponse.object().keys())
        {
            qDebug() << a << jsonResponse.object()[a].toString();
        }
//        if (jsonResponse.object()["Message"].toString().contains("User doesn't exist"))
//            return loginStatus::DOESNT_EXIST;
        if (jsonResponse.object()["code"].toString().contains("400"))
            return loginStatus::INVALID;
    }
    return loginStatus::INVALID;
}

bool BackendClient::logout()
{
    this->jwt.clear();
    return true;
}

QString BackendClient::getJwt() const
{
    return jwt;
}
