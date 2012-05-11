#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <stdexcept>
#include <sstream>
#include "matrix.h"
#include "interval_ext.h"
#include "function_objects.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentCommandIndex(0)
{
    ui->setupUi(this);

    storage = new map_object_storage();
    interpreter = new expression_interpreter(storage);

    load_function_objects(storage);

    //Соединение слотов с сигналами
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->executeButton, SIGNAL(clicked()), this, SLOT(executeCommand()));
    connect(ui->commandLineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));
    connect(ui->commandLineEdit, SIGNAL(), this, SLOT(executeCommand()));

}

MainWindow::~MainWindow()
{
    delete ui;

    delete storage;
    delete interpreter;
}

void MainWindow::executeCommand()
{
    commands << ui->commandLineEdit->text();
    if(commands[currentCommandIndex] != ui->commandLineEdit->text())
    {
        currentCommandIndex = commands.size() - 1;
    }

    std::string command = ui->commandLineEdit->text().toStdString();

    if(command.empty())
    {
        return;
    }

    std::string varName;
    ostringstream sStream;
    try
    {
        varName = interpreter->execute(command);

        sStream << varName << " = " << std::endl;

        stored_object *obj = storage->getObjectByName(varName);
        if(dynamic_cast<numeric_matrix_object*>(obj))
        {
            numeric_matrix_object *num_obj =
                    dynamic_cast<numeric_matrix_object*>(obj);

            print_matr(sStream, num_obj->getMatrix());
        }
        else if(dynamic_cast<interval_matrix_object*>(obj))
        {
            interval_matrix_object *interval_obj =
                    dynamic_cast<interval_matrix_object*>(obj);

            print_matr(sStream, interval_obj->getMatrix());
        }
        else
        {

        }
    }
    catch(std::runtime_error err)
    {
        sStream.clear();
        sStream << err.what();
    }




    sStream << std::endl;

    ui->textEdit->append(ui->commandLineEdit->text());
    ui->textEdit->append(QString(sStream.str().c_str()));
    ui->commandLineEdit->clear();
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if(ui->commandLineEdit->hasFocus() && !commands.isEmpty())
    {
        if(keyEvent->key() == Qt::Key_Up)
        {
            ui->commandLineEdit->setText(commands[currentCommandIndex]);
            if(currentCommandIndex > 0)
                --currentCommandIndex;
        }
        else if(keyEvent->key() == Qt::Key_Down)
        {
            if(currentCommandIndex < commands.size() - 1)
                ++currentCommandIndex;
            ui->commandLineEdit->setText(commands[currentCommandIndex]);
        }
    }
}
