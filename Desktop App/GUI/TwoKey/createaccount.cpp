#include "createaccount.h"
#include "ui_createaccount.h"

createaccount::createaccount(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::createaccount)
{
    ui->setupUi(this);
}

createaccount::~createaccount()
{
    delete ui;
}

void createaccount::on_showpassword_button_clicked()
{
    if(ui->password->echoMode() == QLineEdit::Password){
        ui->password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->password->setEchoMode(QLineEdit::Password);
    }
}

void createaccount::on_login_button_clicked()
{

}

void createaccount::on_createaccount_button_clicked()
{

}
