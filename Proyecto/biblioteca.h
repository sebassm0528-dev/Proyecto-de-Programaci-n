#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class biblioteca;
}
QT_END_NAMESPACE

class biblioteca : public QDialog
{
    Q_OBJECT

public:
    biblioteca(QWidget *parent = nullptr);
    ~biblioteca();

private slots:
    void on_Agregar_button_clicked();

    void on_Eliminar_button_clicked();

    void on_actualizar_button_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::biblioteca *ui;
    void guardarArchivo(); //Para guardar archivo de libros
    void cargarArchivo();  //Para cargar archivo de libros

};
#endif // BIBLIOTECA_H
