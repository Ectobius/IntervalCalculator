#include <sstream>
#include <QtGui>
#include "matrixeditingdialog.h"
#include "ui_matrixeditingdialog.h"
#include "interval_ext.h"

using namespace std;
using namespace int_calc;

/*!
  \brief Конструктор.
  \param parent Родительский виджет.
  \param st Хранилище переменных.
 */
MatrixEditingDialog::MatrixEditingDialog(QWidget *parent,
                                         int_calc::object_storage *st) :
    QDialog(parent),
    stor(st),
    ui(new Ui::MatrixEditingDialog),
    rowsCount(0),
    columnsCount(0),
    cellRegExp("\\s*(\\[\\s*-?\\d+(\\.\\d+)?([eE][+-]\\d+)?\\s*;"
        "\\s*-?\\d+(\\.\\d+)?([eE][+-]\\d+)?\\s*\\]|-?\\d+(\\.\\d+)?([eE][+-]\\d+)?)\\s*")
{
    ui->setupUi(this);

    QRegExp regExp("[a-zA-Z][a-zA-Z0-9]*");
    ui->nameLineEdit->setValidator(new QRegExpValidator(regExp, this));

    //Соединение слотов с сигналами
    connect(ui->rowsSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(changeRowsCount(int)));
    connect(ui->columnsSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(changeColumnsCount(int)));
    connect(ui->tableWidget, SIGNAL(cellChanged(int,int)),
            this, SLOT(checkCellContent(int,int)));
}

MatrixEditingDialog::~MatrixEditingDialog()
{
    delete ui;
}

/*!
  \brief Установка редактируемой матрицы.
  \param matr Указатель на matrix_object, хранящий редактируемую матрицу.
 */
void MatrixEditingDialog::setEditedMatrix(int_calc::matrix_object *matr)
{
    editedMatrix = matr;

    ui->rowsSpinBox->setValue(matr->getRows());
    ui->columnsSpinBox->setValue(matr->getColumns());

    ui->tableWidget->setRowCount(matr->getRows());
    ui->tableWidget->setColumnCount(matr->getColumns());

    ui->tableWidget->clear();
    ostringstream osStream;

    if(dynamic_cast<numeric_matrix_object*>(matr))
    {
        numeric_matrix_object *num_matr =
                dynamic_cast<numeric_matrix_object*>(matr);
        for(size_t i = 0; i != num_matr->getRows(); ++i)
        {
            for(size_t j = 0; j != num_matr->getColumns(); ++j)
            {
                osStream.str(string());
                osStream << num_matr->getMatrix()(i, j);
                QString qstr(osStream.str().c_str());
                if(QTableWidgetItem *item = ui->tableWidget->item(i, j))
                    item->setText(qstr);
                else
                    ui->tableWidget->setItem(i, j, new QTableWidgetItem(qstr));
            }
        }
    }
    else if(dynamic_cast<interval_matrix_object*>(matr))
    {
        interval_matrix_object *interval_matr =
                dynamic_cast<interval_matrix_object*>(matr);
        for(size_t i = 0; i != interval_matr->getRows(); ++i)
        {
            for(size_t j = 0; j != interval_matr->getColumns(); ++j)
            {
                osStream.str(string());
                osStream << interval_matr->getMatrix()(i, j);
                QString qstr(osStream.str().c_str());
                if(QTableWidgetItem *item = ui->tableWidget->item(i, j))
                    item->setText(qstr);
                else
                    ui->tableWidget->setItem(i, j, new QTableWidgetItem(qstr));
            }
        }
    }
}

/*!
  \brief Установка имени редактируемой матрицы.
  \param name Имя редактируемой матрицы.
 */
void MatrixEditingDialog::setMatrixName(const QString &name)
{
    matrixName = name;
    ui->nameLineEdit->setText(name);
}

/*!
  \brief Изменение количества строк таблицы для редактирования матрицы.
  \param cnt Новое количество строк.
 */
void MatrixEditingDialog::changeRowsCount(int cnt)
{
    if(cnt > ui->tableWidget->rowCount())
    {
        for(int i = ui->tableWidget->rowCount(); i != cnt; ++i)
        {
            ui->tableWidget->insertRow(i);
            for(int j = 0; j != ui->tableWidget->columnCount(); ++j)
            {
                ui->tableWidget->setItem(i, j, new QTableWidgetItem("0"));
            }
        }
    }
    else if(cnt < ui->tableWidget->rowCount())
    {
        int oldCnt = ui->tableWidget->rowCount();
        for(int i = cnt; i != oldCnt; ++i)
        {
            ui->tableWidget->removeRow(ui->tableWidget->rowCount() - 1);
        }
    }
}

/*!
  \brief Изменение количества столбцов таблицы для редактирования матрицы.
  \param cnt Новое количество столбцов.
 */
void MatrixEditingDialog::changeColumnsCount(int cnt)
{
    if(cnt > ui->tableWidget->columnCount())
    {
        for(int j = ui->tableWidget->columnCount(); j != cnt; ++j)
        {
            ui->tableWidget->insertColumn(j);
            for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
            {
                ui->tableWidget->setItem(i, j, new QTableWidgetItem("0"));
            }
        }
    }
    else if(cnt < ui->tableWidget->columnCount())
    {
        int oldCnt = ui->tableWidget->columnCount();
        for(int j = cnt; j != oldCnt; ++j)
        {
            ui->tableWidget->removeColumn(ui->tableWidget->columnCount() - 1);
        }
    }
}

/*!
  \brief Проверка значения в ячейке на корректность.
  \param row Номер строки ячейки.
  \param col Номер столбца ячейки.
  \return Если корректно true, иначе false.
 */
bool MatrixEditingDialog::checkCellContent(int row, int col)
{
    if(ui->tableWidget->item(row, col) && !cellRegExp.exactMatch(
                ui->tableWidget->item(row, col)->text()))
    {
        ui->tableWidget->item(row, col)->setText("0");
        QMessageBox::warning(this, "", QString::fromUtf8("Неправильное значение в ячейке"));
        return false;
    }
    return true;
}

/*!
  \brief Обработчик события закрытия окна.
 */
void MatrixEditingDialog::closeEvent(QCloseEvent *closeEvent)
{
    if(!ui->nameLineEdit->hasAcceptableInput())
    {
        QMessageBox::warning(this, "", QString::fromUtf8("Имя имеет неверный формат"));
        ui->nameLineEdit->setFocus();
        closeEvent->ignore();
        return;
    }

    stored_object *obj = stor->getObjectByName(
                ui->nameLineEdit->text().toStdString());
    if(dynamic_cast<function_object*>(obj))
    {
        QMessageBox::warning(this, "", QString::fromUtf8("Недопустимое имя переменной"));
        closeEvent->ignore();
        return;
    }

    for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
        for(int j = 0; j != ui->tableWidget->columnCount(); ++j)
            if(!checkCellContent(i, j))
            {
                closeEvent->ignore();
                return;
            }
    if(saveMatrix())
        closeEvent->accept();
    else
        closeEvent->ignore();
}

/*!
  \brief Сохраняет изменения в редактируемую матрицу.
  \return true в случае успешного сохранения, false иначе.
 */
bool MatrixEditingDialog::saveMatrix()
{
    QRegExp intervalRegExp("\\s*\\[\\s*(-?\\d+(?:\\.\\d+)?(?:[eE][+-]\\d+)?)\\s*;"
            "\\s*(-?\\d+(?:\\.\\d+)?(?:[eE][+-]\\d+)?)\\s*\\]\\s*");
    QRegExp numberRegExp("\\s*-?\\d+(\\.\\d+)?([eE][+-]\\d+)?\\s*");
    matrix_object *savedMatr = 0;
    size_t rows = ui->rowsSpinBox->value();
    size_t columns = ui->columnsSpinBox->value();
    bool isInterval = false;
    for(int i = 0; i != ui->tableWidget->rowCount() && !isInterval; ++i)
        for(int j = 0; j != ui->tableWidget->columnCount() && !isInterval; ++j)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            if(intervalRegExp.exactMatch(item->text()))
                isInterval = true;
        }
    if(isInterval)
    {
        double val1 = 0;
        double val2 = 0;
        interval_matrix_object *intervalMatr =
                new interval_matrix_object(rows, columns);
        for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
            for(int j = 0; j != ui->tableWidget->columnCount(); ++j)
            {
                QTableWidgetItem *item = ui->tableWidget->item(i, j);
                if(intervalRegExp.exactMatch(item->text()))
                {
                    intervalRegExp.indexIn(item->text());
                    val1 = intervalRegExp.cap(1).toDouble();
                    val2 = intervalRegExp.cap(2).toDouble();
                    if(val1 > val2)
                    {
                       QMessageBox::warning(this, "",
                                            QString::fromUtf8("Левый конец интервала "
                                                              "должен быть меньше правого"));
                       delete intervalMatr;
                       return false;
                    }
                    intervalMatr->getMatrix()(i, j) = interval_double(val1, val2);
                }
                else
                {
                    numberRegExp.indexIn(item->text());
                    val1 = numberRegExp.cap(0).toDouble();
                    intervalMatr->getMatrix()(i, j) = val1;
                }
            }
        savedMatr = intervalMatr;
    }
    else
    {
        double val = 0;
        numeric_matrix_object *numericMatr =
                new numeric_matrix_object(rows, columns);
        for(int i = 0; i != ui->tableWidget->rowCount(); ++i)
            for(int j = 0; j != ui->tableWidget->columnCount(); ++j)
            {
                QTableWidgetItem *item = ui->tableWidget->item(i, j);

                numberRegExp.indexIn(item->text());
                val = numberRegExp.cap(0).toDouble();
                numericMatr->getMatrix()(i, j) = val;
            }
        savedMatr = numericMatr;
    }

    if(ui->nameLineEdit->text() !=
            matrixName)
    {
        stor->deleteObject(matrixName.toStdString());
        stor->addObject(ui->nameLineEdit->text().toStdString(), savedMatr);
    }
    else
    {
        stor->addObject(ui->nameLineEdit->text().toStdString(), savedMatr);
    }
    return true;
}
