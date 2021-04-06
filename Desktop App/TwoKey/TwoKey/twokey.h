#ifndef TWOKEY_H
#define TWOKEY_H


#include "usb_communicator.h"

#include <QSocketNotifier>
#include <QMainWindow>
#include <QClipboard>
#include <QShortcut>
#include <QtNetwork>
#include <iostream>
#include <QWidget>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class TwoKey; }
QT_END_NAMESPACE

class TwoKey : public QWidget
{
    Q_OBJECT

public:
    TwoKey(QWidget *parent = nullptr, USB_communicator *usb_comm = nullptr);
    ~TwoKey();

    bool backend_login(QString, QString);
    bool backend_register(QString, QString, QString, QString, QString, QString);
    bool backend_2fa();
    bool backend_logout();

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

private:
    Ui::TwoKey *ui;
    QString jwt;
    USB_communicator *usb_comm;
    QShortcut *returnShortcut;
    QString tokenChallengeResponse;
};
#endif // TWOKEY_H
