#include "twokey.h"
#include "ui_twokey.h"

TwoKey::TwoKey(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TwoKey)
{
    ui->setupUi(this);
}

TwoKey::~TwoKey()
{
    delete ui;
}

