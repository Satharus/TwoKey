#include "twokey.h"
#include "ui_twokey.h"

TwoKey::TwoKey(QWidget *parent, USBCommunicator *usbComm) :
    QWidget(parent)
    , ui(new Ui::TwoKey)
{
    ui->setupUi(this);

    this->backendClient = new BackendClient(usbComm);

    this->browserExtensionThread = new QThread();

    this->signalWrapper = new BrowserExtensionCommunicatorSignalWrapper(backendClient);
    this->browserExtensionComm = new BrowserExtensionCommunicator(signalWrapper);

    browserExtensionComm->moveToThread(browserExtensionThread);

    connect(browserExtensionThread, SIGNAL(started()), browserExtensionComm, SLOT(startServer()));
    connect(browserExtensionThread, SIGNAL(finished()), browserExtensionComm, SLOT(stopServer()));

    connect(signalWrapper, SIGNAL(successfulLogin()), this, SLOT(loginFromBrowserExtension()));

    browserExtensionThread->start();


    ui->twokey_stackedwidget->setCurrentIndex(0);
    ui->manager_save_button->setVisible(false);
    ui->manager_generate_button->setVisible(false);
    ui->manager_logout_button->setVisible(false);
    ui->manager_logout_button->setEnabled(false);
    this->ui->manager_delete_button->setVisible(false);
    this->ui->manager_delete_button->setEnabled(false);

    returnShortcut = new QShortcut(QKeySequence("Return"), ui->login_page);
    QObject::connect(returnShortcut, SIGNAL(activated()), ui->login_button, SLOT(click()));


    passwordDialog = new PasswordDialog(this);
    connect(passwordDialog, SIGNAL(passwordGenerated(QString)), this, SLOT(fillGeneratedPassword(QString)));

    this->closing = false;

    exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(attemptToClose()));

    twoKeySystemTrayMenu = new QMenu(this);
    twoKeySystemTrayMenu->addAction(exitAction);

    twoKeySystemTrayIcon = new QSystemTrayIcon(this);
    twoKeySystemTrayIcon->setContextMenu(twoKeySystemTrayMenu);
    twoKeySystemTrayIcon->setIcon(QIcon(":/Icons/Branding/unicode_key.png"));
    twoKeySystemTrayIcon->show();

    connect(twoKeySystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayIconActivated(QSystemTrayIcon::ActivationReason)));

    this->ui->twokey->setText("<html><head/><body><p><img src=\":/Icons/Branding/Logo_Default.png\" width=\"60\" height=\"60\"/><span style=\" font-size:36pt; font-weight:600;\"> TwoKey</span></p></body></html>");

    this->usbComm = usbComm;
    connect(this->usbComm->usb_notif, SIGNAL(tokenStatusChanged()), this, SLOT(changeStatus()));

    this->usbComm->usb_notif->checkDeviceID();
    this->usbComm->checkForToken();

}

void TwoKey::closeEvent(QCloseEvent *event)
{
    if(closing)
    {
        event->accept();
    }
    else
    {
        this->hide();
        event->ignore();
    }
}

void TwoKey::loadWebsitesList()
{
    ui->manager_accounts_list->clear();
    QStringList websites = this->backendClient->getWebsites();
    this->ui->manager_accounts_list->addItems(websites);
    this->loadAccountsForWebsite(websites[0]);
    this->ui->manager_password->setText(this->credentials[this->ui->manager_accounts_combobox->currentText()]);
}

void TwoKey::loadAccountsForWebsite(QString website)
{
    for (int i = 0 ; i < ui->manager_accounts_combobox->count(); i++)
    {
        ui->manager_accounts_combobox->removeItem(i);
    }
    ui->manager_accounts_combobox->clear();

    this->credentials = this->backendClient->getAccountsForWebsite(website);

    ui->manager_accounts_combobox->setMaxCount(this->credentials.count());

    int i = 0;
    for(auto a : this->credentials.keys())
    {
        if (i == 0)
        {
            ui->manager_website->setText(website);
            ui->manager_password->setText(this->credentials[a]);
        }
        ui->manager_accounts_combobox->addItem(a);
    }
}

TwoKey::~TwoKey()
{
    this->twoKeySystemTrayIcon->hide();
    delete twoKeySystemTrayIcon;
    delete twoKeySystemTrayMenu;
    delete exitAction;
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
    if (ui->login_email->text().isEmpty() || ui->login_password->text().isEmpty())
    {
        QMessageBox::critical(this, "Empty Login Fields", "Please enter your credentials.");
        return;
    }

    if (!usbComm->getTokenStatus())
    {
        QMessageBox::critical(this, "Token not Found", "TwoKey's token is not connected, please connect it to login.");
        return;
    }

    int loginStatus = backendClient->login(ui->login_email->text(), ui->login_password->text());
    if (loginStatus == BackendClient::loginStatus::SUCCESS)
    {
        ui->twokey_stackedwidget->setCurrentIndex(2);
        ui->login_email->clear();
        ui->login_password->clear();
        this->loadWebsitesList();
    }
    else if (loginStatus == BackendClient::loginStatus::INVALID)
    {
        QMessageBox::critical(this, "Invalid credentials!", "The username, password, and/or token is incorrect.");
        ui->login_password->clear();
    }
    else if (loginStatus == BackendClient::loginStatus::NORESPONSE)
    {
        QMessageBox::critical(this, "Communication Error", "No Server Response. Please check your network connection or see if you can load https://twokey.tech");
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
    this->backendClient->addAccount(this->ui->addaccount_website->text(),
                                    this->ui->addaccount_username->text(),
                                    this->ui->addaccount_password->text());

    this->loadWebsitesList();
    this->loadAccountsForWebsite(this->ui->addaccount_website->text());
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
    this->ui->manager_delete_button->setVisible(true);
    this->ui->manager_delete_button->setEnabled(true);
}

void TwoKey::on_manager_save_button_clicked()    //    SAVE INFO BUTTON
{    

    if (ui->manager_password->text().isEmpty() || ui->manager_username->text().isEmpty())
    {
        QMessageBox::critical(this, "Invalid password", "Password can't be empty.");
        return;
    }


    ui->manager_edit_button->setEnabled(true);
    ui->manager_edit_button->setVisible(true);
    ui->manager_website->setReadOnly(true);
    ui->manager_username->setReadOnly(true);
    ui->manager_password->setReadOnly(true);
    ui->manager_generate_button->setVisible(false);
    ui->manager_generate_button->setEnabled(false);
    ui->manager_save_button->setEnabled(false);
    ui->manager_save_button->setVisible(false);
    this->ui->manager_delete_button->setVisible(false);
    this->ui->manager_delete_button->setEnabled(false);

    if (ui->manager_password->text() == oldPassword && ui->manager_username->text() == oldUsername)
    {
        return;
    }

    QString password = this->ui->manager_password->text();
    QString username = this->ui->manager_username->text();
    bool ret = false;
    ret = this->backendClient->changeAccount(this->ui->manager_website->text(), this->oldUsername, username, password);
    if (ret)
    {
        QMessageBox::information(this, "Successful", "The account information was updated successfuly.");
        this->credentials[this->oldUsername] = password;
        if (this->oldUsername != username)
        {
            this->credentials.insert(username, password);
            this->credentials.remove(this->oldUsername);
        }
        this->loadAccountsForWebsite(this->ui->manager_website->text());
    }
    else
    {
       QMessageBox::information(this, "Failed", "The account information failed to update.");
    }
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
        ui->login_showpassword_button->setIcon(QIcon(":/Icons/Buttons/Hide.png"));
    }
    else
    {
        ui->login_password->setEchoMode(QLineEdit::Password);
        ui->login_showpassword_button->setIcon(QIcon(":/Icons/Buttons/Show.png"));
    }
}


//                SECOND page SIGN UP

void TwoKey::on_register_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->register_password->echoMode() == QLineEdit::Password)
    {
        ui->register_password->setEchoMode(QLineEdit::Normal);
        ui->register_showpassword_button->setIcon(QIcon(":/Icons/Buttons/Hide.png"));
    }
    else
    {
        ui->register_password->setEchoMode(QLineEdit::Password);
        ui->register_showpassword_button->setIcon(QIcon(":/Icons/Buttons/Show.png"));
    }
}

//                THIRD page MANAGER

void TwoKey::on_manager_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->manager_password->echoMode() == QLineEdit::Password)
    {
        ui->manager_password->setEchoMode(QLineEdit::Normal);
        ui->manager_showpassword_button->setIcon(QIcon(":/Icons/Buttons/Hide.png"));
    }
    else
    {
        ui->manager_password->setEchoMode(QLineEdit::Password);
        ui->manager_showpassword_button->setIcon(QIcon(":/Icons/Buttons/Show.png"));
    }
}

void TwoKey::on_manager_copypassword_button_clicked()    //    COPY PASSWORD TO CLIPBOARD
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->manager_password->text());
}


void TwoKey::on_manager_generate_button_clicked()    //    GENERATE PASSWORD
{
    passwordDialog->show();
}

//                FORTH page ADD ACCOUNT


void TwoKey::on_addaccount_showpassword_button_clicked()    //     SHOW PASSWORD
{
    if(ui->addaccount_password->echoMode() == QLineEdit::Password)
    {
        ui->addaccount_password->setEchoMode(QLineEdit::Normal);
        ui->addaccount_showpassword_button->setIcon(QIcon(":/Icons/Buttons/Hide.png"));
    }
    else
    {
        ui->addaccount_password->setEchoMode(QLineEdit::Password);
        ui->addaccount_showpassword_button->setIcon(QIcon(":/Icons/Buttons/Show.png"));
    }
}

void TwoKey::on_addaccount_generate_button_clicked()    //    GENERATE PASSWORD
{
     passwordDialog->show();
}

void TwoKey::changeStatus()
{
    this->usbComm->checkForToken();
    if (usbComm->getTokenStatus())
    {
        //Set to green
        this->ui->twokey->setText("<html><head/><body><p><img src=\":/Icons/Branding/Logo_Unlocked.png\" width=\"60\" height=\"60\"/><span style=\" font-size:36pt; font-weight:600;\"> TwoKey</span></p></body></html>");

        this->ui->twokey->setToolTip("TwoKey's token is connected.");

        twoKeySystemTrayIcon->setIcon(QIcon(":/Icons/Branding/Logo_Unlocked.png"));
        twoKeySystemTrayIcon->show();

        setWindowIcon(QIcon(":/Icons/Branding/Logo_Unlocked.png"));
    }
    else
    {
        //Set to red
        this->ui->twokey->setText("<html><head/><body><p><img src=\":/Icons/Branding/Logo_Locked.png\" width=\"60\" height=\"60\"/><span style=\" font-size:36pt; font-weight:600;\"> TwoKey</span></p></body></html>");

        this->ui->twokey->setToolTip("TwoKey's token is disconnected.");

        twoKeySystemTrayIcon->setIcon(QIcon(":/Icons/Branding/Logo_Locked.png"));
        twoKeySystemTrayIcon->show();

        setWindowIcon(QIcon(":/Icons/Branding/Logo_Locked.png"));

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

void TwoKey::loginFromBrowserExtension()
{
    ui->twokey_stackedwidget->setCurrentIndex(2);
}

void TwoKey::on_manager_accounts_combobox_currentIndexChanged(const QString &arg1)
{
    ui->manager_username->setText(arg1);
    this->oldUsername = arg1;
    this->ui->manager_password->setText(this->credentials[arg1]);
    this->oldPassword = this->ui->manager_password->text();
}

void TwoKey::on_manager_accounts_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    (void) previous;
    if (current != nullptr)
    {
        this->loadAccountsForWebsite(current->text());
        this->on_manager_accounts_combobox_currentIndexChanged(ui->manager_accounts_combobox->currentText());
    }
}

void TwoKey::attemptToClose()
{
    closing = true;
    close();
}

void TwoKey::systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        if(isVisible())
        {
            hide();
        }
        else
        {
            show();
            activateWindow();
        }
    }
}

void TwoKey::fillGeneratedPassword(QString password)
{
    if (ui->twokey_stackedwidget->currentIndex() == 2)
    {
        ui->manager_password->setText(password);
    }
    else if (ui->twokey_stackedwidget->currentIndex() == 3)
    {
        ui->addaccount_password->setText(password);
    }
}


void TwoKey::on_manager_password_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        this->ui->passwordWarningLabel->clear();
    }
    else if (!PasswordGenerator::checkPasswordStrength(arg1))
    {
        this->ui->passwordWarningLabel->setText("⚠ Chosen Password is weak");
    }
    else
    {
        this->ui->passwordWarningLabel->setText("✅ Chosen password is strong");
    }
}

void TwoKey::on_manager_delete_button_clicked()
{
    bool ret = this->backendClient->removeAccount(this->ui->manager_website->text(), this->ui->manager_username->text());
    if (ret)
    {
        QMessageBox::information(this, "Successful", "Account removed successfuly.");
        this->loadWebsitesList();
    }
    else
    {
        QMessageBox::information(this, "Failed", "Unable to remove account.");
    }
}


void TwoKey::on_addaccount_password_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        this->ui->addaccount_passwordWarningLabel->clear();
    }
    else if (!PasswordGenerator::checkPasswordStrength(arg1))
    {
        this->ui->addaccount_passwordWarningLabel->setText("⚠ Chosen Password is weak");
    }
    else
    {
        this->ui->addaccount_passwordWarningLabel->setText("✅ Chosen password is strong");
    }
}

