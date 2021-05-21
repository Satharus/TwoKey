#include "passwordgenerator.h"

#include <QTime>

PasswordGenerator::PasswordGenerator() : QObject ()
{
    randomGenerator = new QRandomGenerator(static_cast<quint32>(QTime::currentTime().msec()));
}

bool PasswordGenerator::checkPasswordStrength(QString password)
{
    if (password.length() < 9)
    {
        return false;
    }

    if (!(password.contains(QRegExp("[a-z]")) && password.contains(QRegExp("[A-Z]"))))
    {
        return false;
    }

    if (!(password.contains(QRegExp("[0-9]"))))
    {
        return false;
    }

    bool containsSpecial = false;
    for (QChar a : this->specialCharacters)
    {
        if (password.contains(a))
            containsSpecial = true;
    }

    if (!containsSpecial && password.length() < 11) return false;

    return true;
}

QString PasswordGenerator::generatePassword(int mask, int length)
{
    if (mask == 0 || length < 4) return QString("");

    int specialCharacterCount = 0;
    QString password;
    int generatedMask = 0;


    QChar c;
    for (int i = 0; i < length; i++)
    {
        c = '/';
        QChar options[4] = {};
        if (mask&LOWERCASE)
        {
            options[0] = static_cast<QChar>(randomGenerator->bounded('a', 'z'+1));
            c = options[0];
        }

        if (mask&UPPERCASE)
        {
            int chance = randomGenerator->bounded(0, 2);
            options[1] = static_cast<QChar>(randomGenerator->bounded('A', 'Z'+1));
            if (chance == 1)
                c = options[1];
        }

        if (mask&NUMBERS)
        {
            int chance = randomGenerator->bounded(0, 2);
            options[2] = static_cast<QChar>(randomGenerator->bounded('0', '9'+1));
            if (chance == 1)
                c = options[2];
        }

        if (mask&SPECIAL_CHARS)
        {
            int chance = randomGenerator->bounded(0, 2);
            int idx = randomGenerator->bounded(0, 9);
            options[3] = this->specialCharacters[idx];
            if (chance == 1)
                c = options[3];
        }

        if (c == '/')
        {
repeat:     int idx = randomGenerator->bounded(0, 4);
            if (options[idx] == '\0') goto repeat;
            c = options[idx];
        }


        bool isSpecial = false;

        for (int i = 0; i < 8; i++)
        {
            if (c == specialCharacters[i])
                isSpecial = true;
        }


        if (c.isLower())        generatedMask |= LOWERCASE;
        else if (c.isUpper())   generatedMask |= UPPERCASE;
        else if (c.isNumber())  generatedMask |= NUMBERS;
        else if (isSpecial)
        {
            generatedMask |= SPECIAL_CHARS;
            specialCharacterCount++;
        }

        if (isSpecial && specialCharacterCount <= static_cast<double>(ceil(0.2*length)))
            password.append(c);
        else if (isSpecial)
            i--;
        else
            password.append(c);
    }

    if (generatedMask == mask) return password;
    else return generatePassword(mask, length);
}
