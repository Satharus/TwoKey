#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include "passwordgenerator.h"

#include <QMessageBox>
#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = nullptr);
    ~PasswordDialog();

signals:
     void passwordGenerated(QString);

private slots:
    void on_LengthSlider_valueChanged(int value);

    void on_UppercasCheckBox_stateChanged(int arg1);

    void on_LowercaseCheckBox_stateChanged(int arg1);

    void on_NumbersCheckBox_stateChanged(int arg1);

    void on_SpecialCharacterCheckBox_stateChanged(int arg1);

    void on_RegenrateButton_clicked();

    void on_AcceptButton_clicked();

    void on_LengthSlider_sliderReleased();

private:
    Ui::PasswordDialog *ui;
    PasswordGenerator *generator;

    int passwordLength;
    int passwordMask;
};

#endif // PASSWORDDIALOG_H
