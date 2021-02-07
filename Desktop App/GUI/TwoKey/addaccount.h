#ifndef ADDACCOUNT_H
#define ADDACCOUNT_H

#include <QMainWindow>

namespace Ui {
class addaccount;
}

class addaccount : public QMainWindow
{
    Q_OBJECT

public:
    explicit addaccount(QWidget *parent = nullptr);
    ~addaccount();

private slots:
    void on_addaccount_showpassword_clicked();

private:
    Ui::addaccount *ui;
};

#endif // ADDACCOUNT_H
