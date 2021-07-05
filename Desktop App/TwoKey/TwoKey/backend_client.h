#ifndef BACKEND_CLIENT_H
#define BACKEND_CLIENT_H

#include "usb_communicator.h"

#include <QCryptographicHash>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>
#include <QtNetwork>

#define KEY_LEN  32

class BackendClient : public QObject
{
    Q_OBJECT
public:
    BackendClient(USBCommunicator *usbComm);
    int login(QString email, QString password);
    bool _register(QString firstName, QString lastName, QString email, QString username, QString password, QString serial);
    int _2fa();
    bool logout();

    QStringList getWebsites();
    QHash<QString, QString> getAccountsForWebsite(QString website);
    bool changeAccount(QString website, QString oldUsername, QString username, QString password);
    bool addAccount(QString website, QString username, QString password);
    bool removeAccount(QString website, QString username);

    QString getJwt() const;
    QString encryptWithKey(std::string text);
    QString decryptWithKey(std::string encryptedText);

    enum loginStatus
    {
        SUCCESS       =     0,
        INVALID       =     1,
        NORESPONSE    =     2
    };
    Q_ENUM(loginStatus)

private:
    QString jwt;
    QString masterpassword;
    QByteArray encryptionkey;
    QString salt;
    QString tokenChallengeResponse;
    USBCommunicator *usbComm;
    QStringList parseJSONArray(QString arrayString);

};


class CHashGenerator
{
public:
    std::string calculateHMACHash(const char* password, const char* salt, const char* data, int iterations);
    char* generateKey(const char* password , const unsigned char* salt, int iterations);

private:
    char pbkdf2Key[64];
    char hash[41];
};

#endif // BACKEND_CLIENT_H
