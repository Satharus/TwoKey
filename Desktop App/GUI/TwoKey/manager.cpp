#include "manager.h"
#include "ui_manager.h"
#include "qclipboard.h"

manager::manager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::manager)
{
    ui->setupUi(this);
}

manager::~manager()
{
    delete ui;
}

void manager::on_showpassword_button_clicked()
{
    if(ui->password->echoMode() == QLineEdit::Password){
        ui->password->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->password->setEchoMode(QLineEdit::Password);
    }
}

void manager::on_edit_button_clicked()
{
    ui->website->setReadOnly(false);
    ui->username->setReadOnly(false);
    ui->password->setReadOnly(false);
    ui->generate_button->setEnabled(true);
    ui->save_button->setEnabled(true);
}

void manager::on_copypassword_button_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->password->text());
}

void manager::on_save_button_clicked()
{
    ui->website->setReadOnly(true);
    ui->username->setReadOnly(true);
    ui->password->setReadOnly(true);
    ui->save_button->setEnabled(false);
}
