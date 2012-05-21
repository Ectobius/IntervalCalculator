#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <stdexcept>
#include <sstream>
#include "matrix.h"
#include "interval_ext.h"
#include "function_objects.h"
#include "signalingstorage.h"
#include "matrixsavingdialog.h"
#include "systemmodelingdialog.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentCommandIndex(0)
{
    ui->setupUi(this);



    SignalingStorage *signStor = new SignalingStorage();
    connect(&signStor->changedSignal, SIGNAL(signal()), this, SLOT(updateVariableList()));
    storage = signStor;

    interpreter = new expression_interpreter(storage);

    load_function_objects(storage);

    matrixEditingDialog = new MatrixEditingDialog(this, storage);

    //Соединение слотов с сигналами
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->executeButton, SIGNAL(clicked()), this, SLOT(executeCommand()));
    connect(ui->commandLineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(editMatrix(QListWidgetItem*)));
    connect(ui->addVarPushButton, SIGNAL(clicked()),
            this, SLOT(addMatrix()));
    connect(ui->delVarPushButton, SIGNAL(clicked()),
            this, SLOT(deleteMatrix()));
    connect(ui->actionSaveVariables, SIGNAL(triggered()),
            this, SLOT(saveVariables()));
    connect(ui->actionLoadVariables, SIGNAL(triggered()),
            this, SLOT(loadVariables()));

    connect(ui->actionModeling, SIGNAL(triggered()),
            this, SLOT(modelSystem()));

    //
    QList<int> splitSisez;
    splitSisez.append(this->width() * 0.25);
    splitSisez.append(this->width() * 0.75);
    ui->splitter->setSizes(splitSisez);

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

            sStream << num_obj->getMatrix();
        }
        else if(dynamic_cast<interval_matrix_object*>(obj))
        {
            interval_matrix_object *interval_obj =
                    dynamic_cast<interval_matrix_object*>(obj);

            sStream << interval_obj->getMatrix();
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

void MainWindow::updateVariableList()
{
    ui->listWidget->clear();
    for(named_object obj = storage->getFirst(); obj.getObject(); obj = storage->getNext())
    {
        if(dynamic_cast<matrix_object*>(obj.getObject()))
        {
            ui->listWidget->addItem(QString(obj.getName().c_str()));
        }
    }

}

void MainWindow::editMatrix(QListWidgetItem *item)
{
    std::string name = item->text().toStdString();
    matrixEditingDialog->setMatrixName(QString(name.c_str()));
    stored_object *obj = storage->getObjectByName(name);
    if(dynamic_cast<matrix_object*>(obj))
    {
        matrix_object *matrObj =
                dynamic_cast<matrix_object*>(obj);
        matrixEditingDialog->setEditedMatrix(matrObj);
        matrixEditingDialog->exec();
    }
}

void MainWindow::addMatrix()
{
    std::string name("untitled");
    if(storage->getObjectByName(name))
    {
        int num = 1;
        while(storage->getObjectByName(name +
                                       QString::number(num++).toStdString()));
        name += QString::number(num - 1).toStdString();
    }
    numeric_matrix_object *numObj =
            new numeric_matrix_object(1, 1);
    numObj->getMatrix()(0, 0) = 0;
    storage->addObject(name, numObj);

    matrixEditingDialog->setMatrixName(QString(name.c_str()));
    matrixEditingDialog->setEditedMatrix(numObj);
    matrixEditingDialog->exec();
}

void MainWindow::deleteMatrix()
{
    if(ui->listWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, "",
                             QString::fromUtf8("Выделите элемент списка"));
        return;
    }
    QListWidgetItem *item =
            ui->listWidget->selectedItems()[0];
    storage->deleteObject(item->text().toStdString());
}

void MainWindow::saveVariables()
{
    MatrixSavingDialog savingDialog(storage, this);
    savingDialog.exec();
}

void MainWindow::loadVariables()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть"),
                                         ".", QString::fromUtf8("Текстовые файлы (*.txt);;Все файлы (*.*)"));
    if(fileName.isEmpty())
    {
       return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "",
                             QString::fromUtf8("Ошибка при открытии файла"));
        return;
    }

    QList<named_object> objList;
    QTextStream inStream(&file);

    try
    {
        loadVariablesToList(objList, inStream);
        file.close();
    }
    catch(runtime_error err)
    {
        QMessageBox::warning(this, "",
                             QString::fromUtf8("Ошибка при чтении файла"));
        return;
    }

    for(int i = 0; i != objList.size(); ++i)
    {
        stored_object *obj = storage->getObjectByName(objList[i].getName());
        if(dynamic_cast<function_object*>(obj))
        {
            delete objList[i].getObject();

        }
        else
        {
            storage->addObject(objList[i].getName(), objList[i].getObject());
        }
    }
}

void MainWindow::loadVariablesToList(QList<named_object> &lst, QTextStream &inStream)
{
    QString curStr;
    QString name;
    int rows = 0, columns = 0;
    interval_matrix_object *intervalObj = 0;
    QRegExp elemRegExp("\\s*(\\[\\s*-?\\d+(\\.\\d+)?([eE][+-]\\d+)?\\s*;"
        "\\s*-?\\d+(\\.\\d+)?([eE][+-]\\d+)?\\s*\\]|-?\\d+(\\.\\d+)?([eE][+-]\\d+)?)\\s*");
    QRegExp intervalRegExp("\\s*\\[\\s*(-?\\d+(?:\\.\\d+)?(?:[eE][+-]\\d+)?)\\s*;"
            "\\s*(-?\\d+(?:\\.\\d+)?(?:[eE][+-]\\d+)?)\\s*\\]\\s*");
    QRegExp numberRegExp("\\s*-?\\d+(\\.\\d+)?([eE][+-]\\d+)?\\s*");
    bool end = false;
    while(!end)
    {
        do
        {
            inStream >> curStr;
        }
        while(curStr.isEmpty() && !inStream.atEnd());
        if(inStream.atEnd())
            break;
        name = curStr;
        inStream >> rows >> columns;
        if(rows <= 0 || columns <= 0)
        {
            throw runtime_error("File has wrong format");
        }

        intervalObj = new interval_matrix_object(rows, columns);
        inStream.readLine();
        QString elemStr;
        bool isInterval = false;
        for(int i = 0; i != rows; ++i)
        {
            int index = 0;
            curStr = inStream.readLine();
            for(int j = 0; j != columns; ++j)
            {
                index = elemRegExp.indexIn(curStr, index);
                elemStr = elemRegExp.cap(0);
                if(intervalRegExp.exactMatch(elemStr))
                {
                    isInterval = true;
                    intervalRegExp.indexIn(elemStr);
                    double val1 = intervalRegExp.cap(1).toDouble();
                    double val2 = intervalRegExp.cap(2).toDouble();
                    intervalObj->getMatrix()(i, j) = d_interval(val1, val2);
                }
                else if(numberRegExp.exactMatch(elemStr))
                {
                    numberRegExp.indexIn(elemStr);
                    double val = numberRegExp.cap(0).toDouble();
                    intervalObj->getMatrix()(i, j) = val;
                }
                else
                {
                    throw runtime_error("File reading error");
                }
                index += elemRegExp.matchedLength();
            }
        }

        if(isInterval)
        {
            lst.push_back(named_object(name.toStdString(), intervalObj));
        }
        else
        {
            numeric_matrix_object *numObj =
                    new numeric_matrix_object(intervalObj->getRows(), intervalObj->getColumns());
            for(int i = 0; i != intervalObj->getRows(); ++i)
                for(int j = 0; j != intervalObj->getColumns(); ++j)
                    numObj->getMatrix()(i, j) = intervalObj->getMatrix()(i, j).lower();
            delete intervalObj;
            lst.push_back(named_object(name.toStdString(), numObj));
        }
    }
}

void MainWindow::modelSystem()
{
    SystemModelingDialog modelingDialog(storage, this);
    connect(&dynamic_cast<SignalingStorage*>(storage)->changedSignal, SIGNAL(signal()),
            &modelingDialog, SLOT(updateMatrixComboBox()));
    modelingDialog.exec();
}
