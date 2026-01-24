#include "registrologin.h"
#include "./ui_registrologin.h"
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


registroLogin::registroLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registroLogin)
{
    ui->setupUi(this);
}

registroLogin::~registroLogin()
{
    delete ui;
}

//------------------------------------------------------------------------------------------------------------------

void registroLogin::on_pushButton_clicked()
{
    QString usuario = ui->usuario_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    QString confirmarP = ui->confPassw_lineEdit->text();

//Valida que todos los campos esten llenos-------------------------------------------------------------
    if(usuario.isEmpty() || password.isEmpty() || confirmarP.isEmpty()){
        QMessageBox::warning(this,"Campos vacíos","Por favor llene todos los campos");
        return;
    }

//Para validar que las contraseñas coincidan-----------------------------------------------------------
    if(password != confirmarP){
        QMessageBox::warning(this,"- Error -","Las contraseñas no coinciden");
        return;
    }

//Para guardar en un archivo los usuarios registrados--------------------------------------------------
    QString ruta = QCoreApplication::applicationDirPath() + "/usuarios.txt";

    // Validar si un usuario esta duplicado--------------------------------------------
    QFile file(ruta);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd()){
            QString linea = in.readLine();
            QStringList datos = linea.split(";");

            if(datos[0] == usuario){
                QMessageBox::warning(this,"R E G I S T R O","Este usuario ya existe");
                file.close();
                return;
            }
        }
        file.close();
    }

    //Pra guardar usuario registrado--------------------------------------------------
    if(file.open(QIODevice::Append | QIODevice::Text)){
        QTextStream out(&file);
        out << usuario << ";" << password << "\n";
        file.close();
    }

    QMessageBox::information(this,"R E G I S T R O","Usuario registrado con exito");

    //Para volver al login------------------------------------------------------------
    MainWindow *login = new MainWindow();
    login->show();
    this->close();
}

//--------------------------------------------------------------------------------------------

//Boton para regresar al login
void registroLogin::on_atras_pushButton_clicked()
{
    MainWindow *login = new MainWindow;
    login->show();
    this->close();
}

