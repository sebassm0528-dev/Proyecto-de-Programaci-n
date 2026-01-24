#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "registrologin.h"
#include "biblioteca.h"
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_crear_button_clicked()
{
    registroLogin registro = new registroLogin;
    this->hide();
    registro.exec();
}

//-------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_ButtonLogin_clicked()
{
    QString usuario = ui->usuario_lineEdit->text();
    QString password = ui->password_lineEdit->text();

//Para validar que se llenen todos los campos---------------------------------------------------------------
    if(usuario.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this,"Campos vacíos","Por favor llene todos los campos");
        return;
    }

//Para el archivo que guarda todos los usuarios-------------------------------------------------------------
    QString ruta = QCoreApplication::applicationDirPath() + "/usuarios.txt";

    QFile file(ruta);
    bool encontrado = false;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);

        while(!in.atEnd()){
            QString linea = in.readLine().trimmed();

            if(linea.isEmpty()) continue;

            QStringList datos = linea.split(";");

            if(datos.size() < 2) continue;

            if(datos[0] == usuario && datos[1] == password){
                encontrado = true;
                break;
            }
        }
        file.close();
    }

//Para que si el usuario es correcto vaya a la ventana de la Biblioteca-------------------------------------
    if(encontrado){
        QMessageBox::information(this,"Login","Bienvenido   - " + usuario + " -");
        biblioteca b = new biblioteca;
        this->hide();
        b.exec();
    }else{
        QMessageBox::warning(this,"- Error -","Usuario o contraseña incorrectos");
    }
}

