#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}


void login::on_showpassword_button_clicked()
{
    if(ui->login_password->echoMode() == QLineEdit::Password){
        ui->login_password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->login_password->setEchoMode(QLineEdit::Password);
    }
}
