#include "twokey.h"
#include "ui_twokey.h"

TwoKey::TwoKey(QWidget *parent, USBCommunicator *usbComm) :
    QWidget(parent)
    , ui(new Ui::TwoKey)
{
    ui->setupUi(this);

    this->backendClient = new BackendClient(usbComm);
    qDebug() << (void *) this->backendClient;

    this->usbComm = usbComm;
    connect(this->usbComm->usb_notif, SIGNAL(tokenStatusChanged()), this, SLOT(changeStatus()));

    this->usbComm->usb_notif->checkDeviceID();
    this->usbComm->checkForToken();


    this->browserExtensionThread = new QThread();

    this->signalWrapper = new BrowserExtensionCommunicatorSignalWrapper(backendClient);
    this->browserExtensionComm = new BrowserExtensionCommunicator(signalWrapper);

    browserExtensionComm->moveToThread(browserExtensionThread);

    connect(browserExtensionThread, SIGNAL(started()), browserExtensionComm, SLOT(startServer()));
    connect(browserExtensionThread, SIGNAL(finished()), browserExtensionComm, SLOT(stopServer()));

    connect(signalWrapper, SIGNAL(testSignal(QString)), this, SLOT(messageBoxTest(QString)));

    browserExtensionThread->start();


    ui->twokey_stackedwidget->setCurrentIndex(0);
    ui->manager_save_button->setVisible(false);
    ui->manager_generate_button->setVisible(false);
    ui->manager_logout_button->setVisible(false);
    ui->manager_logout_button->setEnabled(false);

    returnShortcut = new QShortcut(QKeySequence("Return"), ui->login_page);
    QObject::connect(returnShortcut, SIGNAL(activated()), ui->login_button, SLOT(click()));
}

TwoKey::~TwoKey()
{
    delete usbComm;
    delete returnShortcut;
    delete ui;
}

/*
 _   _    ___     _____ ____    _  _____ ___ ___  _   _
| \ | |  / \ \   / |_ _/ ___|  / \|_   _|_ _/ _ \| \ | |
|  \| | / _ \ \ / / | | |  _  / _ \ | |  | | | | |  \| |
| |\  |/ ___ \ V /  | | |_| |/ ___ \| |  | | |_| | |\  |
|_| \_/_/   \_\_/  |___\____/_/   \_|_| |___\___/|_| \_|

*/

//                FIRST page LOGIN

void TwoKey::on_login_button_clicked()
{
    if (!usbComm->getTokenStatus())
    {
        QMessageBox::critical(this, "Token not Found", "TwoKey's token is not connected, please connect it to login.");
        return;
    }

    int loginStatus = backendClient->login(ui->login_email->text(), ui->login_password->text());
    if (loginStatus == BackendClient::loginStatus::SUCCESS)
    {
        ui->twokey_stackedwidget->setCurrentIndex(2); // LOGIN BUTTON
        ui->login_email->clear();
        ui->login_password->clear();
    }
    else if (loginStatus == BackendClient::loginStatus::INVALID)
    {
        QMessageBox::critical(this, "Invalid credentials!", "The username or password you have entered is incorrect.");
        ui->login_password->clear();
    }
    else if (loginStatus == BackendClient::loginStatus::DOESNT_EXIST)
    {
        QMessageBox::critical(this, "Invalid account!", "The user doesn't exist.");
        ui->login_password->clear();
    }
}

void TwoKey::on_createaccount_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(1); // CREATE ACCOUNT BUTTON
}

//                SECOND page SIGN UP

void TwoKey::on_register_button_clicked()
{
    if (backendClient->_register(ui->register_firstname->text(),
                         ui->register_lastname->text(),
                         ui->register_email->text(),
                         ui->register_username->text(),
                         ui->register_password->text(),
                         ui->register_serial->text()))
    {
        ui->twokey_stackedwidget->setCurrentIndex(0); // CREATE ACCOUNT BUTTON
        ui->register_firstname->clear();
        ui->register_lastname->clear();
        ui->register_email->clear();
        ui->register_username->clear();
        ui->register_password->clear();
        ui->register_serial->clear();
    }
}

void TwoKey::on_register_back_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(0); // LOGIN BUTTON
}

//                THIRD page MANAGER

void TwoKey::on_manager_addaccount_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(3); // ADD ACCOUNT BUTTON
}

void TwoKey::on_manager_logout_button_clicked()
{
    backendClient->logout();
    ui->twokey_stackedwidget->setCurrentIndex(0);
}

//                FORTH page ADD ACCOUNT

void TwoKey::on_addaccount_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(2); // ADD ACCOUNT BUTTON
}

void TwoKey::on_addaccount_cancel_button_clicked()
{
    ui->twokey_stackedwidget->setCurrentIndex(2); // CANCEL BUTTON
}

/*
 __  __    _    _   _    _    ____ _____ ____     _____ ____ ___ _____    ___ _   _ _____ ___     ____  _   _ _____ _____ ___  _   _ ____
|  \/  |  / \  | \ | |  / \  / ___| ____|  _ \   | ____|  _ |_ _|_   _|  |_ _| \ | |  ___/ _ \   | __ )| | | |_   _|_   _/ _ \| \ | / ___|
| |\/| | / _ \ |  \| | / _ \| |  _|  _| | |_) |  |  _| | | | | |  | |     | ||  \| | |_ | | | |  |  _ \| | | | | |   | || | | |  \| \___ \
| |  | |/ ___ \| |\  |/ ___ | |_| | |___|  _ <   | |___| |_| | |  | |     | || |\  |  _|| |_| |  | |_) | |_| | | |   | || |_| | |\  |___) |
|_|  |_/_/   \_|_| \_/_/   \_\____|_____|_| \_\  |_____|____|___| |_|    |___|_| \_|_|   \___/   |____/ \___/  |_|   |_| \___/|_| \_|____/

*/

//                ONLY    THIRD page MANAGER

void TwoKey::on_manager_edit_button_clicked()    //    EDIT INFO BUTTON
{
    ui->manager_edit_button->setEnabled(false);
    ui->manager_edit_button->setVisible(false);
    ui->manager_website->setReadOnly(false);
    ui->manager_username->setReadOnly(false);
    ui->manager_password->setReadOnly(false);
    ui->manager_generate_button->setVisible(true);
    ui->manager_generate_button->setEnabled(true);
    ui->manager_save_button->setVisible(true);
    ui->manager_save_button->setEnabled(true);
}

void TwoKey::on_manager_save_button_clicked()    //    SAVE INFO BUTTON
{
    ui->manager_edit_button->setEnabled(true);
    ui->manager_edit_button->setVisible(true);
    ui->manager_website->setReadOnly(true);
    ui->manager_username->setReadOnly(true);
    ui->manager_password->setReadOnly(true);
    ui->manager_generate_button->setVisible(false);
    ui->manager_generate_button->setEnabled(false);
    ui->manager_save_button->setEnabled(false);
    ui->manager_save_button->setVisible(false);
}

/*
 ____   _    ____ ______        _____  ____  ____     ____   _____  _    ____  _   _ _____ _____ ___  _   _ ____
|  _ \ / \  / ___/ ___\ \      / / _ \|  _ \|  _ \   | __ ) / _ \ \/ /  | __ )| | | |_   _|_   _/ _ \| \ | / ___|
| |_) / _ \ \___ \___ \\ \ /\ / | | | | |_) | | | |  |  _ \| | | \  /   |  _ \| | | | | |   | || | | |  \| \___ \
|  __/ ___ \ ___) ___) |\ V  V /| |_| |  _ <| |_| |  | |_) | |_| /  \   | |_) | |_| | | |   | || |_| | |\  |___) |
|_| /_/   \_|____|____/  \_/\_/  \___/|_| \_|____/   |____/ \___/_/\_\  |____/ \___/  |_|   |_| \___/|_| \_|____/

*/

//                FIRST page LOGIN

void TwoKey::on_login_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->login_password->echoMode() == QLineEdit::Password)
    {
        ui->login_password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->login_password->setEchoMode(QLineEdit::Password);
    }
}


//                SECOND page SIGN UP

void TwoKey::on_register_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->register_password->echoMode() == QLineEdit::Password)
    {
        ui->register_password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->register_password->setEchoMode(QLineEdit::Password);
    }
}

//                THIRD page MANAGER

void TwoKey::on_manager_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->manager_password->echoMode() == QLineEdit::Password)
    {
        ui->manager_password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->manager_password->setEchoMode(QLineEdit::Password);
    }
}

void TwoKey::on_manager_copypassword_button_clicked()    //    COPY PASSWORD TO CLIPBOARD
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->manager_password->text());
}

void TwoKey::on_manager_generate_button_clicked()    //    GENERATE PASSWORD
{
    ui->manager_password->setText("PASSWORDMANAGER");
}

//                FORTH page ADD ACCOUNT


void TwoKey::on_addaccount_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->addaccount_password->echoMode() == QLineEdit::Password)
    {
        ui->addaccount_password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->addaccount_password->setEchoMode(QLineEdit::Password);
    }
}

void TwoKey::on_addaccount_generate_button_clicked()    //    GENERATE PASSWORD
{
    ui->manager_password->setText("PASSWORDMANAGER");
}

void TwoKey::changeStatus()
{
    this->usbComm->checkForToken();
    if (usbComm->getTokenStatus())
    {
        //Set to green
        ui->statusLabel->setPixmap(QPixmap("://Icons/StatusIcons/greenStatusIconCircle.png").scaled(ui->statusLabel->maximumWidth(),
                                                                                     ui->statusLabel->maximumHeight(),
                                                                                     Qt::KeepAspectRatio));
        this->ui->statusLabel->setToolTip("TwoKey's token is connected.");
    }
    else
    {
        //Set to red
        ui->statusLabel->setPixmap(QPixmap("://Icons/StatusIcons/redStatusIconCircle.png").scaled(ui->statusLabel->maximumWidth(),
                                                                                     ui->statusLabel->maximumHeight(),
                                                                                     Qt::KeepAspectRatio));
        this->ui->statusLabel->setToolTip("TwoKey's token is disconnected.");
        backendClient->logout();
        ui->twokey_stackedwidget->setCurrentIndex(0);

    }
}

void TwoKey::on_twokey_stackedwidget_currentChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->manager_logout_button->setVisible(true);
        ui->manager_logout_button->setEnabled(true);
    }
    else
    {
        ui->manager_logout_button->setVisible(false);
        ui->manager_logout_button->setEnabled(false);
    }
}

void TwoKey::messageBoxTest(QString pwd)
{
    QMessageBox::information(this, "Data recieved from browser extension", "Adam Zahran is based and redpilled\n The Recieved token is " + QString(pwd));
}

void TwoKey::on_manager_accounts_combobox_currentIndexChanged(const QString &arg1)
{
    ui->manager_username->setText(arg1);
    if (arg1 == "newuser@gmail.com")
    {
        ui->manager_password->setText("y8aGfuH$D6vH");
    }
    else if (arg1 == "myotheraccount@gmail.com")
    {
        ui->manager_password->setText("mEbab6j$sjmT");
    }
    else if (arg1 == "2017170000@cis.asu.edu.eg")
    {
        ui->manager_password->setText("2oC$guWyQrTs");
    }
}

void TwoKey::on_manager_accounts_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    (void) previous;
    if (current->text() == "ASU")
    {
        ui->manager_accounts_combobox->setMaxCount(1);
        ui->manager_accounts_combobox->setItemText(0, "2017170000@cis.asu.edu.eg");
        ui->manager_website->setText("cis.asu.edu.eg");
    }
    else if (current->text() == "LinkedIn")
    {
        ui->manager_accounts_combobox->setMaxCount(1);
        ui->manager_accounts_combobox->setItemText(0, "newuser@gmail.com");
        ui->manager_website->setText("linkedin.com");
    }
    else if (current->text() == "Google")
    {
        ui->manager_accounts_combobox->setMaxCount(2);
        ui->manager_accounts_combobox->setItemText(0, "newuser@gmail.com");
        ui->manager_accounts_combobox->addItem("myotheraccount@gmail.com");
        ui->manager_website->setText("google.com");
    }

   this->on_manager_accounts_combobox_currentIndexChanged(ui->manager_accounts_combobox->currentText());
}

