#ifndef REGISTROLOGIN_H
#define REGISTROLOGIN_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class registroLogin;
}
QT_END_NAMESPACE

class registroLogin : public QDialog
{
    Q_OBJECT

public:
    registroLogin(QWidget *parent = nullptr);
    ~registroLogin();

private slots:
    void on_pushButton_clicked();

    void on_atras_pushButton_clicked();

private:
    Ui::registroLogin *ui;
};
#endif // REGISTROLOGIN_H
