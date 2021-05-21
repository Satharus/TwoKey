#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);

    generator = new PasswordGenerator();

    this->passwordLength = 14;
    this->passwordMask = PasswordGenerator::UPPERCASE | PasswordGenerator::LOWERCASE | PasswordGenerator::NUMBERS;

    connect(ui->LengthSlider, SIGNAL(valueChanged(int)), ui->LengthSpinBox, SLOT(setValue(int)));
    connect(ui->LengthSpinBox, SIGNAL(valueChanged(int)), ui->LengthSlider, SLOT(setValue(int)));
    this->ui->LengthSlider->setValue(this->passwordLength);

    this->on_RegenrateButton_clicked();
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
    delete generator;
}

void PasswordDialog::on_LengthSlider_valueChanged(int value)
{
    this->passwordLength = value;
}


void PasswordDialog::on_UppercasCheckBox_stateChanged(int arg1)
{
    (void) arg1;
    this->passwordMask ^= PasswordGenerator::PasswordStrength::UPPERCASE;
}


void PasswordDialog::on_LowercaseCheckBox_stateChanged(int arg1)
{
    (void) arg1;
    this->passwordMask ^= PasswordGenerator::PasswordStrength::LOWERCASE;
}


void PasswordDialog::on_NumbersCheckBox_stateChanged(int arg1)
{
    (void) arg1;
    this->passwordMask ^= PasswordGenerator::PasswordStrength::NUMBERS;
}


void PasswordDialog::on_SpecialCharacterCheckBox_stateChanged(int arg1)
{
    (void) arg1;
    this->passwordMask ^= PasswordGenerator::PasswordStrength::SPECIAL_CHARS;
}


void PasswordDialog::on_RegenrateButton_clicked()
{
    if (this->passwordMask == 0 || this->passwordLength == 0)
    {
        QMessageBox::critical(this, "Invalid options", "Please choose valid settings.");
        return;
    }

    this->ui->passwordLabel->setText(generator->generatePassword(this->passwordMask, this->passwordLength));
}


void PasswordDialog::on_AcceptButton_clicked()
{
    emit passwordGenerated(this->ui->passwordLabel->text());
    close();
}


void PasswordDialog::on_LengthSlider_sliderReleased()
{
    this->on_RegenrateButton_clicked();
}
