#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QMainWindow>

namespace Ui {
class createaccount;
}

class createaccount : public QMainWindow
{
    Q_OBJECT

public:
    explicit createaccount(QWidget *parent = nullptr);
    ~createaccount();

private slots:
    void on_showpassword_button_clicked();

private:
    Ui::createaccount *ui;
};

#endif // CREATEACCOUNT_H
