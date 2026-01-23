#include "biblioteca.h"
#include "./ui_biblioteca.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QStyledItemDelegate>
#include <QFile>
#include <QHeaderView>

//-------------------------------------------------------------------------------------------------------------

//Validación para ID (permita solo numeros)
class NumericDelegate : public QStyledItemDelegate {
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const override {

        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new QIntValidator(0, 999999, editor));
        editor->setMaxLength(6);
        return editor;
    }
};

//-------------------------------------------------------------------------------------------------------------

//Validación para Año de publicación (permita solo maximo 4 números "ej:2024")
class YearDelegate : public QStyledItemDelegate {
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const override {

        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new QIntValidator(0, 2026, editor));
        editor->setMaxLength(4);
        return editor;
    }
};

//-------------------------------------------------------------------------------------------------------------

biblioteca::biblioteca(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::biblioteca)
{
    ui->setupUi(this);

    //Validaciones para Año de publicación
    ui->publi_lineEdit->setValidator(new QIntValidator(0, 2026, this));
    ui->tableWidget->setItemDelegateForColumn(3, new YearDelegate());
    ui->publi_lineEdit->setMaxLength(4);

    //Validación para ID
    ui->ID_lineEdit->setValidator(new QIntValidator(0, 999999, this));
    ui->tableWidget->setItemDelegateForColumn(0, new YearDelegate());
    ui->ID_lineEdit->setMaxLength(6);


    //Para tableWidget (forma de la tabla)
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Título", "Autor", "Año de publicación"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->horizontalHeader()->setTextElideMode(Qt::ElideNone);

    //Para cargar los libros registrados
    cargarArchivo();
}

//-------------------------------------------------------------------------------------------------------------

biblioteca::~biblioteca()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------------------

void biblioteca::on_Agregar_button_clicked()
{
//Declaración de variables---------------------------------------------------------------------------------
    QString ID = ui->ID_lineEdit->text();
    QString titulo = ui->titulo_lineEdit->text();
    QString autor = ui->autor_lineEdit->text();
    QString publicacion = ui->publi_lineEdit->text();

//Validación de que se llenen todos los campos-------------------------------------------------------------
    if(ID.isEmpty() || titulo.isEmpty() || autor.isEmpty() || publicacion.isEmpty()){
        QMessageBox::warning(this, "Campos vacíos", "Por favor llene todos los campos.");
        return;
    }

//Validación para que ID permita solo números enteros------------------------------------------------------
    bool esNumero;
    ID.toInt(&esNumero);
    if (!esNumero) {
        QMessageBox::warning(this, "- Error -", "En ID solo se permiten números enteros.");
        return;
    }

//Validación para que Año de publicación reciba hasta 2026-------------------------------------------------
    bool ok;
    int anio = publicacion.toInt(&ok);
    if(!ok || anio > 2026){
        QMessageBox::warning(this, "- Error -", "El año no puede ser mayor a 2026");
        return;
    }

//Hace que llene el tableWidget----------------------------------------------------------------------------
    const int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(ID));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(titulo));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(autor));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(publicacion));

    //Para guardar los libros registrados
    guardarArchivo();

//Limpia los lineEdit despues de pulsar el boton guardar---------------------------------------------------
    ui->ID_lineEdit->clear();
    ui->titulo_lineEdit->clear();
    ui->autor_lineEdit->clear();
    ui->publi_lineEdit->clear();
}

//-------------------------------------------------------------------------------------------------------------

//Boton de eliminar libro
void biblioteca::on_Eliminar_button_clicked()
{
    int fila = ui->tableWidget->currentRow();
    if (fila == -1) {
        QMessageBox::warning(this,"- Error -", "Seleccione un libro para eliminar");
        return;
    }

    //Para eliminar en el tableWidget
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());

    //Para limpiar los lineEdit despues de eliminar
    ui->ID_lineEdit->clear();
    ui->titulo_lineEdit->clear();
    ui->autor_lineEdit->clear();
    ui->publi_lineEdit->clear();

    guardarArchivo();
}

//-------------------------------------------------------------------------------------------------------------

//Archivo para guardar los libros que se registran
void biblioteca::guardarArchivo()
{
    QFile file("libros.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            for(int j=0;j<ui->tableWidget->columnCount();j++)
            {
                out << ui->tableWidget->item(i,j)->text() << ";";
            }
            out << "\n";
        }
        file.close();
    }
}

//-------------------------------------------------------------------------------------------------------------

//Para hacer que carguen todos los libros guardados en el tableWidget
void biblioteca::cargarArchivo()
{
    QFile file("libros.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        ui->tableWidget->setRowCount(0);

        while(!in.atEnd())
        {
            QString linea = in.readLine();
            QStringList datos = linea.split(";");

            int fila = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(fila);

            for(int i=0;i<4;i++)
            {
                ui->tableWidget->setItem(fila,i,new QTableWidgetItem(datos[i]));
            }
        }
        file.close();
    }
}

//----------------------------------------------------------------------------------------------------------

void biblioteca::on_actualizar_button_clicked()
{
    //Para validar que seleccione un libro para actualizar
    int fila = ui->tableWidget->currentRow();
    if (fila == -1) {
        QMessageBox::warning(this,"- Error -", "Seleccione un libro para actualizar");
        return;
    }

    //Para actualizar en el archivo los libros
    QFile archivo("libros.txt");

    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QStringList lineas;
    QTextStream in(&archivo);

    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList datos = linea.split(";");

        if (datos[0] == ui->ID_lineEdit->text()) {
            linea = ui->ID_lineEdit->text() + ";" +
                    ui->titulo_lineEdit->text() + ";" +
                    ui->autor_lineEdit->text() + ";" +
                    ui->publi_lineEdit->text();
        }

        lineas.append(linea);
    }

    archivo.close();

    // Reescribir archivo
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&archivo);
    for (const QString &l : lineas)
        out << l << "\n";

    archivo.close();

    QMessageBox::information(this, "OK", "Libro actualizado correctamente");

    cargarArchivo();

    //Para limpiar los lineEdit despues de actualizar un libro
    ui->ID_lineEdit->clear();
    ui->titulo_lineEdit->clear();
    ui->autor_lineEdit->clear();
    ui->publi_lineEdit->clear();
}

//----------------------------------------------------------------------------------------------------------

void biblioteca::on_tableWidget_cellClicked(int row, int column)
{
    //Para actualizar la tabla en el tableWidget
    ui->ID_lineEdit->setText(ui->tableWidget->item(row, 0)->text());
    ui->titulo_lineEdit->setText(ui->tableWidget->item(row, 1)->text());
    ui->autor_lineEdit->setText(ui->tableWidget->item(row, 2)->text());
    ui->publi_lineEdit->setText(ui->tableWidget->item(row, 3)->text());
}


