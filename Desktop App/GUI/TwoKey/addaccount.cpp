#include "addaccount.h"
#include "ui_addaccount.h"

addaccount::addaccount(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::addaccount)
{
    ui->setupUi(this);
}

addaccount::~addaccount()
{
    delete ui;
}

void addaccount::on_addaccount_showpassword_clicked()
{
    if(ui->addaccount_password->echoMode() == QLineEdit::Password){
        ui->addaccount_password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->addaccount_password->setEchoMode(QLineEdit::Password);
    }
}

void addaccount::on_cancel_button_clicked()
{

}

void addaccount::on_addaccount_button_clicked()
{

}
