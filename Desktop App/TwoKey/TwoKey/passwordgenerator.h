#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <QRandomGenerator>
#include <QObject>

class PasswordGenerator : public QObject
{
    Q_OBJECT
private:
    QChar specialCharacters[9] = {'!', '@', '#', '$', '%', '^', '&', '*'};
    QRandomGenerator *randomGenerator;
public:
    PasswordGenerator();
    bool checkPasswordStrength(QString password);
    QString generatePassword(int mask, int length);


    enum PasswordStrength
    {
        NUMBERS           =     1,  //0b0001
        SPECIAL_CHARS     =     2,  //0b0010
        LOWERCASE         =     4,  //0b0100
        UPPERCASE         =     8   //0b1000
    };
    Q_ENUM(PasswordStrength)
};

#endif // PASSWORDGENERATOR_H
