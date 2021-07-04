#include "backend_client.h"

BackendClient::BackendClient(USBCommunicator *usbComm) : QObject ()
{
    this->usbComm = usbComm;
}

int BackendClient::login(QString email, QString password)
{
    QNetworkRequest request(QUrl("https://twokey.tech/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    bool validResponse = true;

    QCryptographicHash *sha256sum =  new QCryptographicHash(QCryptographicHash::Algorithm::Sha256);

    QByteArray password_hash = sha256sum->hash(password.toUtf8(), QCryptographicHash::Algorithm::Sha256);

    qDebug() << password_hash.toHex();
    json.insert("email", email);
    json.insert("password", QString(password_hash.toHex()));
    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());
//    reply->ignoreSslErrors();
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();

    if (response_data.isEmpty())
    {
        validResponse = false;
    }

    int loginStatus;
    if (validResponse)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

        QString challenge = jsonResponse.object()["challenge"].toString();
        this->jwt = jsonResponse.object()["Access-token"].toString();
        qDebug() << "Recieved Challenge: " << challenge;

        usbComm->writeToToken(challenge.toStdString().c_str());
        tokenChallengeResponse = usbComm->readFromToken();

        QByteArray response = QByteArray::fromHex(tokenChallengeResponse.toUtf8());

        qDebug() << "Message: " + challenge;
        qDebug() << "Response:" + response.toHex(',');

        loginStatus = _2fa();
    }
    else
    {
        qDebug() << "No server response";
        loginStatus = BackendClient::loginStatus::NORESPONSE;
    }

    delete reply;
    delete sha256sum;
    return  loginStatus;
}



bool BackendClient::_register(QString firstName, QString lastName, QString email, QString username, QString password, QString serial)
{
    QNetworkRequest request(QUrl("https://twokey.tech/reg"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("first_name", firstName);
    json.insert("last_name", lastName);
    json.insert("email", email);
    json.insert("username", username);
    json.insert("serial", serial);


    QCryptographicHash *sha256sum =  new QCryptographicHash(QCryptographicHash::Algorithm::Sha256);

    QByteArray password_hash = sha256sum->hash(password.toUtf8(), QCryptographicHash::Algorithm::Sha256);

    json.insert("password", QString(password_hash.toHex()));
    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

    qDebug() << "\nResponse: " <<  jsonResponse;


    delete sha256sum;
    if (jsonResponse.object()["code"].toString() == "200")
    {
            return true;
    }
    else return false;
}

int BackendClient::_2fa()
{
    QNetworkRequest request(QUrl("https://twokey.tech/2fa"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", jwt);
    json.insert("challenge", QString(tokenChallengeResponse.toUtf8().toBase64()));
    qDebug() << "Encrypted Challenge: " << tokenChallengeResponse;
    qDebug() << "Base64 Challenge: " << QString(tokenChallengeResponse.toUtf8().toBase64());
    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

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

QStringList BackendClient::parseJSONArray(QString arrayString)
{
    QStringList arrayElements;
    for (int i = 0; i < arrayString.length(); i++)
    {
        if (arrayString[i] == '[')
        {
            continue;
        }
        else if (arrayString[i] == ']')
        {
            continue;
        }
        if (arrayString[i] == '"')
        {
            i++;
            QString currentWord;
            while(arrayString[i] != '"')
            {
                currentWord.append(arrayString[i]);
                i++;
            }
            arrayElements.append(currentWord);
        }
    }
    return arrayElements;
}

QStringList BackendClient::getWebsites()
{
    QNetworkRequest request(QUrl("https://twokey.tech/view-urls"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", this->jwt);
    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);


    QStringList websites =  this->parseJSONArray(jsonResponse.object()["data"].toString());  //jsonResponse.object()["data"].toString().split(',');

    return websites;
}

QHash<QString, QString> BackendClient::getAccountsForWebsite(QString website)
{
    QHash<QString, QString> accounts;
    QNetworkRequest request(QUrl("https://twokey.tech/view-accounts"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", this->jwt);
    json.insert("url", website);
    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

    QStringList credentials =  this->parseJSONArray(jsonResponse.object()["data"].toString());

    for (int i = 0; i < credentials.length() - 1; i+=2)
    {
        accounts.insert(credentials[i], credentials[i+1]);
    }

    return accounts;
}

bool BackendClient::changeAccount(QString website, QString oldUsername, QString username, QString password)
{
    QNetworkRequest request(QUrl("https://twokey.tech/update"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", this->jwt);
    json.insert("url", website);
    json.insert("old_username", oldUsername);
    json.insert("username", username);
    json.insert("password", password);

    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

    if (jsonResponse.object()["code"].toString() == "200")
    {
        return true;
    }
    else return false;
}

bool BackendClient::addAccount(QString website, QString username, QString password)
{
    QNetworkRequest request(QUrl("https://twokey.tech/Add-acc"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", this->jwt);
    json.insert("url", website);
    json.insert("username", username);
    json.insert("password", password);

    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

    if (jsonResponse.object()["code"].toString() == "200")
    {
        return true;
    }
    else return false;
}

bool BackendClient::removeAccount(QString website, QString username)
{
    QNetworkRequest request(QUrl("https://twokey.tech/delete-acc"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", this->jwt);
    json.insert("url", website);
    json.insert("username", username);

    QNetworkAccessManager nam;

    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    while (!reply->isFinished())
    {
        qApp->processEvents();
    }

    QByteArray response_data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

    if (jsonResponse.object()["code"].toString() == "200")
    {
        return true;
    }
    else return false;
}

QString BackendClient::getJwt() const
{
    return jwt;
}
