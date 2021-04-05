#ifndef TWOKEY_H
#define TWOKEY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TwoKey; }
QT_END_NAMESPACE

class TwoKey : public QWidget
{
    Q_OBJECT

public:
    TwoKey(QWidget *parent = nullptr);
    ~TwoKey();

private:
    Ui::TwoKey *ui;
};
#endif // TWOKEY_H
