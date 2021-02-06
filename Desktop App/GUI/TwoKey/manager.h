#ifndef MANAGER_H
#define MANAGER_H

#include <QMainWindow>

namespace Ui {
class manager;
}

class manager : public QMainWindow
{
    Q_OBJECT

public:
    explicit manager(QWidget *parent = nullptr);
    ~manager();

private slots:
    void on_showpassword_button_clicked();

    void on_edit_button_clicked();

    void on_copypassword_button_clicked();

    void on_save_button_clicked();

private:
    Ui::manager *ui;
};

#endif // MANAGER_H
