#include "backend_client.h"

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

// base64 encoding
std::string base64_encode(char const* bytes_to_encode, int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';

    }

    return ret;

}

// base64 decoding
std::string base64_decode(std::string & encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

std::string aes_256_cbc_encode(const std::string& password, const std::string& data)
{
    // Set iv to character 0 by default here
    unsigned char iv[AES_BLOCK_SIZE] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

    AES_KEY aes_key;
    if (AES_set_encrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
    {
        //assert(false);
        return "";
    }
    std::string strRet;
    std::string data_bak = data;
    unsigned int data_length = data_bak.length();

    int padding = 0;
    if (data_bak.length() % (AES_BLOCK_SIZE) > 0)
    {
        padding = AES_BLOCK_SIZE - data_bak.length() % (AES_BLOCK_SIZE);
    }
    data_length += padding;
    while (padding > 0)
    {
        data_bak += '\0';
        padding--;
    }

    for (unsigned int i = 0; i < data_length / (AES_BLOCK_SIZE); i++)
    {
        std::string str16 = data_bak.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        unsigned char out[AES_BLOCK_SIZE];
        ::memset(out, 0, AES_BLOCK_SIZE);
        AES_cbc_encrypt((const unsigned char*)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);
        strRet += std::string((const char*)out, AES_BLOCK_SIZE);
    }
    return strRet;
}

std::string aes_256_cbc_decode(const std::string& password, const std::string& strData)
{
    // Set iv to character 0 by default here
    unsigned char iv[AES_BLOCK_SIZE] = { '0','0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };

    AES_KEY aes_key;
    if (AES_set_decrypt_key((const unsigned char*)password.c_str(), password.length() * 8, &aes_key) < 0)
    {
        //assert(false);
        return "";
    }
    std::string strRet;
    for (unsigned int i = 0; i < strData.length() / AES_BLOCK_SIZE; i++)
    {
        std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        unsigned char out[AES_BLOCK_SIZE];
        ::memset(out, 0, AES_BLOCK_SIZE);
        AES_cbc_encrypt((const unsigned char*)str16.c_str(), out, AES_BLOCK_SIZE, &aes_key, iv, AES_DECRYPT);
        strRet += std::string((const char*)out, AES_BLOCK_SIZE);
    }
    return strRet;
}



BackendClient::BackendClient(USBCommunicator *usbComm) : QObject ()
{
    this->usbComm = usbComm;
}

int BackendClient::login(QString email, QString password)
{
    CHashGenerator keyDeriver;

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

        this->masterpassword = password;
        this->encryptionkey = QByteArray::fromHex(QString(keyDeriver.generateKey(password.toStdString().c_str(), (unsigned char *) password_hash.toStdString().substr(1,32).c_str(), 10000)).toUtf8());

        qDebug() << this->decryptWithKey(this->encryptWithKey(password.toStdString()).toStdString());


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
    for (int i = 0; i < masterpassword.length(); i++)
    {
        masterpassword[i] = '\0';
    }
    this->masterpassword.clear();
    for (int i = 0; i < encryptionkey.length(); i++)
    {
        encryptionkey[i] = '\0';
    }
    this->encryptionkey.clear();
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

    for (int i = 0; i < websites.length(); i++)
    {
        websites[i] = this->decryptWithKey(websites[i].toStdString());
    }

    return websites;
}

QHash<QString, QString> BackendClient::getAccountsForWebsite(QString website)
{
    QHash<QString, QString> accounts;
    QNetworkRequest request(QUrl("https://twokey.tech/view-accounts"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", this->jwt);
    json.insert("url", this->encryptWithKey(website.toStdString()));
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
        accounts.insert(this->decryptWithKey(credentials[i].toStdString()), this->decryptWithKey(credentials[i+1].toStdString()));
    }

    return accounts;
}

bool BackendClient::changeAccount(QString website, QString oldUsername, QString username, QString password)
{
    QNetworkRequest request(QUrl("https://twokey.tech/update"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("Access-token", this->jwt);
    json.insert("url", this->encryptWithKey(website.toStdString()));
    json.insert("old_username", this->encryptWithKey(oldUsername.toStdString()));
    json.insert("username", this->encryptWithKey(username.toStdString()));
    json.insert("password", this->encryptWithKey(password.toStdString()));

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
    json.insert("url", this->encryptWithKey(website.toStdString()));
    json.insert("username", this->encryptWithKey(username.toStdString()));
    json.insert("password", this->encryptWithKey(password.toStdString()));

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
    json.insert("url", this->encryptWithKey(website.toStdString()));
    json.insert("username", this->encryptWithKey(username.toStdString()));

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

QString BackendClient::encryptWithKey(std::string text)
{
    std::string str_encode = aes_256_cbc_encode(this->encryptionkey.toStdString(), text);
    std::string str_encode_base64 = base64_encode(str_encode.c_str(), str_encode.length());

    return QString(str_encode_base64.c_str());
}

QString BackendClient::decryptWithKey(std::string encryptedText)
{

    std::string str_decode_base64 = base64_decode(encryptedText);
    std::string str_decode = aes_256_cbc_decode(this->encryptionkey.toStdString(), str_decode_base64);

    return QString(str_decode.c_str());
}

char* CHashGenerator::generateKey(const char* password , const unsigned char* salt, int iterations)
{
    size_t size;
    unsigned char *out;
    out = (unsigned char *) malloc(sizeof(unsigned char) * KEY_LEN);

    if( PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), salt, strlen((char*)salt), iterations, KEY_LEN, out) != 0 )
    {
        for(size=0;size<KEY_LEN;size++)
        {
            sprintf(&pbkdf2Key[size*2], "%02x", (unsigned int)out[size]);
        }
    }
    else
    {
        fprintf(stderr, "PKCS5_PBKDF2_HMAC_SHA1 failed\n");
    }
    free(out);
    return pbkdf2Key;
}

std::string CHashGenerator::calculateHMACHash(const char* password, const char* salt, const char* data, int iterations)
{
    // The key to hash
    std::string saltStr(salt);
    const unsigned char* unsignedSalt = (unsigned char*)saltStr.c_str();
    char* key = generateKey(password,unsignedSalt,iterations);

    // The data that we’re going to hash using HMAC
    std::string dataVal(data);
    const unsigned char* unsignedData = (unsigned char*)dataVal.c_str();
    unsigned char* digest;

    // I have used sha1 hash engine here.
    digest = HMAC(EVP_sha1(), key, strlen(key), unsignedData, strlen((char *)unsignedData), NULL, NULL);

    // Length of string depends on the chosen hash engine for example with the chosen hash engine  i.e SHA1 it produces a 20-byte hash value which rendered as 40 characters.
    // Length of the string need to be changed as per hash engine used.
    for(int i = 0; i < 20; i++)
    {
     sprintf(&hash[i*2], "%02x", (unsigned int)digest[i]);
    }
    return std::string(hash);
}



