#ifndef TWOKEY_H
#define TWOKEY_H


#include "browser_extension_communicator.h"
#include "usb_communicator.h"
#include "backend_client.h"

#include <QListWidgetItem>
#include <QMessageBox>
#include <QMainWindow>
#include <QClipboard>
#include <QShortcut>
#include <iostream>
#include <QWidget>
#include <QThread>
#include <QPixmap>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class TwoKey; }
QT_END_NAMESPACE

class TwoKey : public QWidget
{
    Q_OBJECT

public:
    TwoKey(QWidget *parent = nullptr, USBCommunicator *usbComm = nullptr);
    ~TwoKey();

private slots:
    // NAVIGATION
    void on_login_button_clicked();

    void on_createaccount_button_clicked();

    void on_register_button_clicked();

    void on_register_back_button_clicked();

    void on_manager_addaccount_button_clicked();

    void on_manager_logout_button_clicked();

    void on_addaccount_button_clicked();

    void on_addaccount_cancel_button_clicked();

    // MANAGER EDIT INFO BUTTONS
    void on_manager_edit_button_clicked();

    void on_manager_save_button_clicked();

    // PASSWORD BOX BUTTONS
    void on_login_showpassword_button_clicked();

    void on_register_showpassword_button_clicked();

    void on_manager_showpassword_button_clicked();

    void on_manager_copypassword_button_clicked();

    void on_manager_generate_button_clicked();

    void on_addaccount_showpassword_button_clicked();

    void on_addaccount_generate_button_clicked();

    void changeStatus();

    void on_twokey_stackedwidget_currentChanged(int arg1);

    void messageBoxTest(QString pwd);

    void on_manager_accounts_combobox_currentIndexChanged(const QString &arg1);

    void on_manager_accounts_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::TwoKey *ui;
    USBCommunicator *usbComm;

    BackendClient *backendClient;

    QThread *browserExtensionThread;
    BrowserExtensionCommunicator *browserExtensionComm;
    BrowserExtensionCommunicatorSignalWrapper *signalWrapper;
    QShortcut *returnShortcut;
};
#endif // TWOKEY_H
