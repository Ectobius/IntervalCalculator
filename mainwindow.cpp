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

/*!
  \brief Конструктор.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentCommandIndex(0)
{
    ui->setupUi(this);

    //Инициализация treeWidget
    ui->treeWidget->setColumnCount(2);
    QStringList labels;
    labels.append(trUtf8("Имя"));
    labels.append(trUtf8("Размер"));
    ui->treeWidget->setHeaderLabels(labels);


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
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(editMatrix(QTreeWidgetItem*)));
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
    connect(ui->actionClearConsole, SIGNAL(triggered()),
            this, SLOT(clearConsole()));
    connect(ui->actionDeleteVariables, SIGNAL(triggered()),
            this, SLOT(deleteVariables()));

    connect(ui->actionAboutQt, SIGNAL(triggered()),
            this, SLOT(aboutQt()));
    connect(ui->actionAboutProgram, SIGNAL(triggered()),
            this, SLOT(about()));

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

/*!
  \brief Осуществляет действия по выполнению введенной команды.
 */
void MainWindow::executeCommand()
{
    commands << ui->commandLineEdit->text();
    if (commands[currentCommandIndex] != ui->commandLineEdit->text())
    {
        currentCommandIndex = commands.size() - 1;
    }

    std::string command = ui->commandLineEdit->text().toStdString();

    if (command.empty())
    {
        return;
    }

    expression_result *exp_res = 0;
    std::string varName;
    ostringstream sStream;
    try
    {
        exp_res = interpreter->execute(command);

        if (assign_result *assign_res =
                dynamic_cast<assign_result*>(exp_res))
        {
            varName = assign_res->getVarName();

            sStream << varName << " = " << std::endl;

            stored_object *obj = storage->getObjectByName(varName);
            if (dynamic_cast<numeric_matrix_object*>(obj))
            {
                numeric_matrix_object *num_obj =
                        dynamic_cast<numeric_matrix_object*>(obj);

                sStream << num_obj->getMatrix();
            }
            else if (dynamic_cast<interval_matrix_object*>(obj))
            {
                interval_matrix_object *interval_obj =
                        dynamic_cast<interval_matrix_object*>(obj);

                sStream << interval_obj->getMatrix();
            }
            else
            {

            }
        }
        else if (command_result *command_res =
                 dynamic_cast<command_result*>(exp_res))
        {            
            if (command_res->getCommandName() == "#func")
            {
                for (named_object obj = storage->getFirst(); obj.getObject(); obj = storage->getNext())
                {
                    if (dynamic_cast<function_object*>(obj.getObject()))
                    {
                        sStream << obj.getName() << endl;
                    }
                }
            }
            else
            {
                sStream << "Unknown command" << endl;
            }
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

/*!
  \brief Обработчик события нажатия клавиши.

  Обрабатывает нажатия клавиш - стрелок вверх и вниз для
  перемещения по списку введенных команд.
 */
void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (ui->commandLineEdit->hasFocus() && !commands.isEmpty())
    {
        if (keyEvent->key() == Qt::Key_Up)
        {
            ui->commandLineEdit->setText(commands[currentCommandIndex]);
            if (currentCommandIndex > 0)
                --currentCommandIndex;
        }
        else if (keyEvent->key() == Qt::Key_Down)
        {
            if (currentCommandIndex < commands.size() - 1)
                ++currentCommandIndex;
            ui->commandLineEdit->setText(commands[currentCommandIndex]);
        }
    }
}

/*!
  \brief Обновляет список переменных.
 */
void MainWindow::updateVariableList()
{
    ui->treeWidget->clear();
    QList<QTreeWidgetItem*> items;
    QStringList columns;
    for (named_object obj = storage->getFirst(); obj.getObject(); obj = storage->getNext())
    {
        if (matrix_object* matr_obj =
                dynamic_cast<matrix_object*>(obj.getObject()))
        {
            columns.clear();
            columns.append(QString::fromStdString(obj.getName()));
            columns.append(trUtf8("%1x%2").arg(matr_obj->getRows()).arg(matr_obj->getColumns()));
            items.append(new QTreeWidgetItem(columns));
        }
    }
    ui->treeWidget->insertTopLevelItems(0, items);
}

/*!
  \brief Редактирование матрицы.

  Вызывает окно редактирования матрицы для редактирования уже
  существующей матрицы, отображаемой в списке переменных.

  \param item Выделенный элемент списка.
 */
void MainWindow::editMatrix(QTreeWidgetItem *item)
{
    std::string name = item->text(0).toStdString();

    matrixEditingDialog->setMatrixName(QString(name.c_str()));
    stored_object *obj = storage->getObjectByName(name);
    if (dynamic_cast<matrix_object*>(obj))
    {
        matrix_object *matrObj =
                dynamic_cast<matrix_object*>(obj);
        matrixEditingDialog->setEditedMatrix(matrObj);
        matrixEditingDialog->exec();
    }
}

/*!
  \brief Добавление матрицы.

  Создает новую переменную и вызывает окно для ее редактирования.
 */
void MainWindow::addMatrix()
{
    std::string name("untitled");
    if (storage->getObjectByName(name))
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

/*!
  \brief Удаление матрицы.

  Удаляет переменную, соответствующую выделенному элементу списка.
 */
void MainWindow::deleteMatrix()
{
    if (ui->treeWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, "",
                             QString::fromUtf8("Выделите элемент списка"));
        return;
    }
    QTreeWidgetItem *item =
            ui->treeWidget->selectedItems()[0];
    storage->deleteObject(item->text(0).toStdString());
}

/*!
  \brief Сохранение переменных в файл.

  Вызывает диалоговое окно сохранения переменных в файл.
 */
void MainWindow::saveVariables()
{
    MatrixSavingDialog savingDialog(storage, this);
    savingDialog.exec();
}

/*!
  \brief Загрузка переменных из файла.

  Вызывает диалоговое окно выбора файла и существляет загрузку
  переменных из этого файла.
 */
void MainWindow::loadVariables()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть"),
                                         ".", QString::fromUtf8("Текстовые файлы (*.txt);;Все файлы (*.*)"));
    if (fileName.isEmpty())
    {
       return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
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
    catch (runtime_error err)
    {
        QMessageBox::warning(this, "",
                             QString::fromUtf8("Ошибка при чтении файла"));
        return;
    }

    for (int i = 0; i != objList.size(); ++i)
    {
        stored_object *obj = storage->getObjectByName(objList[i].getName());
        if (dynamic_cast<function_object*>(obj))
        {
            delete objList[i].getObject();

        }
        else
        {
            storage->addObject(objList[i].getName(), objList[i].getObject());
        }
    }
}

/*!
  \brief Считывает переменные в список из текстового потока.
  \param lst Список, в который нужно загрузить переменные.
  \param inStream Текстовый поток, откуда нужно считать переменные.
 */
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
    while (!end)
    {
        do
        {
            inStream >> curStr;
        }
        while (curStr.isEmpty() && !inStream.atEnd());
        if (inStream.atEnd())
            break;
        name = curStr;
        inStream >> rows >> columns;
        if (rows <= 0 || columns <= 0)
        {
            throw runtime_error("File has wrong format");
        }

        intervalObj = new interval_matrix_object(rows, columns);
        inStream.readLine();
        QString elemStr;
        bool isInterval = false;
        for (int i = 0; i != rows; ++i)
        {
            int index = 0;
            curStr = inStream.readLine();
            for (int j = 0; j != columns; ++j)
            {
                index = elemRegExp.indexIn(curStr, index);
                elemStr = elemRegExp.cap(0);
                if (intervalRegExp.exactMatch(elemStr))
                {
                    isInterval = true;
                    intervalRegExp.indexIn(elemStr);
                    double val1 = intervalRegExp.cap(1).toDouble();
                    double val2 = intervalRegExp.cap(2).toDouble();
                    intervalObj->getMatrix()(i, j) = interval_double(val1, val2);
                }
                else if (numberRegExp.exactMatch(elemStr))
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

        if (isInterval)
        {
            lst.push_back(named_object(name.toStdString(), intervalObj));
        }
        else
        {
            numeric_matrix_object *numObj =
                    new numeric_matrix_object(intervalObj->getRows(), intervalObj->getColumns());
            for (int i = 0; i != intervalObj->getRows(); ++i)
                for (int j = 0; j != intervalObj->getColumns(); ++j)
                    numObj->getMatrix()(i, j) = intervalObj->getMatrix()(i, j).lower();
            delete intervalObj;
            lst.push_back(named_object(name.toStdString(), numObj));
        }
    }
}

/*!
  \brief Вызывает окно моделирования систем дифференциальных уравнений.
 */
void MainWindow::modelSystem()
{
    SystemModelingDialog modelingDialog(storage, this);
    connect(&dynamic_cast<SignalingStorage*>(storage)->changedSignal, SIGNAL(signal()),
            &modelingDialog, SLOT(updateMatrixComboBox()));
    modelingDialog.exec();
}

/*!
  \brief Очищает консоль.
 */
void MainWindow::clearConsole()
{
    ui->textEdit->clear();
}

/*!
  \brief Удаляет все переменные.
 */
void MainWindow::deleteVariables()
{
    vector<string> deletedVar;
    for (named_object obj = storage->getFirst(); obj.getObject(); obj = storage->getNext())
    {
        if (dynamic_cast<matrix_object*>(obj.getObject()))
        {
            deletedVar.push_back(obj.getName());
        }
    }

    for (vector<string>::iterator iter = deletedVar.begin();
         iter != deletedVar.end(); ++iter)
    {
        storage->deleteObject(*iter);
    }
}

/*!
  \brief Вызывает окно "О программе".
 */
void MainWindow::about()
{
    QMessageBox::about(this, trUtf8("О программе"),
                       trUtf8("<h2>IntervalCalculator</h2>"
                              "<p>Автор: студент гр. ПОВТ-71 Поповцев А.В., ФИТ, АлтГТУ, 2012</p>"));
}

/*!
  \brief Вызывает окно "О Qt".
 */
void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}
