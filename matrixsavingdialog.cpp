#include <QtGui>
#include <sstream>
#include "interval_ext.h"
#include "matrixsavingdialog.h"
#include "ui_matrixsavingdialog.h"

using namespace int_calc;

MatrixSavingDialog::MatrixSavingDialog(int_calc::object_storage *stor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatrixSavingDialog),
    storage(stor)
{
    ui->setupUi(this);

    loadVariables();

    selectAllVariables();

    //Соединение слотов с сигналами
    connect(ui->selectAllPushButton, SIGNAL(clicked()),
            this, SLOT(selectAllVariables()));
    connect(ui->cancelAllPushButton, SIGNAL(clicked()),
            this, SLOT(cancelAllVariables()));
    connect(ui->selectFileButton, SIGNAL(clicked()),
            this, SLOT(selectFile()));
    connect(ui->savePushButton, SIGNAL(clicked()),
            this, SLOT(saveVariables()));
}

MatrixSavingDialog::~MatrixSavingDialog()
{
    delete ui;
}

void MatrixSavingDialog::loadVariables()
{
    for(named_object obj = storage->getFirst(); obj.getObject(); obj = storage->getNext())
    {
        if(dynamic_cast<matrix_object*>(obj.getObject()))
        {
            QCheckBox *checkBox =
                    new QCheckBox(QString(obj.getName().c_str()), this);
            QListWidgetItem *item = new QListWidgetItem();
            ui->listWidget->addItem(item);
            ui->listWidget->setItemWidget(item, checkBox);
        }
    }
}

void MatrixSavingDialog::setListCheckState(bool state)
{
    for(int i = 0; i != ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if(QCheckBox *checkBox = dynamic_cast<QCheckBox*>(
                    ui->listWidget->itemWidget(item)))
        {
            checkBox->setChecked(state);
        }
    }
}

void MatrixSavingDialog::selectAllVariables()
{
    setListCheckState(true);
}

void MatrixSavingDialog::cancelAllVariables()
{
    setListCheckState(false);
}

void MatrixSavingDialog::selectFile()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, QString::fromUtf8("Сохранить как"), ".",
                                         QString::fromUtf8("Текстовые файлы (*.txt);;Все файлы (*.*)"));
    if(!fileName.isEmpty())
        ui->fileNameLineEdit->setText(fileName);
}

void MatrixSavingDialog::saveVariables()
{
    QFile file(ui->fileNameLineEdit->text());
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "",
                             QString::fromUtf8("Ошибка при сохранении файла"));
        return;
    }

    std::ostringstream sStream;
    QTextStream out(&file);
    for(int i = 0; i != ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if(QCheckBox *checkBox = dynamic_cast<QCheckBox*>(
                    ui->listWidget->itemWidget(item)))
        {
            if(checkBox->isChecked())
            {
                sStream.str(string());
                std::string objName = checkBox->text().toStdString();
                stored_object *obj = storage->getObjectByName(objName);
                if(dynamic_cast<numeric_matrix_object*>(obj))
                {
                    numeric_matrix_object *num_obj =
                            dynamic_cast<numeric_matrix_object*>(obj);
                    sStream << num_obj->getMatrix();
                    out << checkBox->text() << endl;
                    out << num_obj->getRows() << " " << num_obj->getColumns() << endl;
                    out << QString(sStream.str().c_str()) << endl;
                }
                else if(dynamic_cast<interval_matrix_object*>(obj))
                {
                    interval_matrix_object *interval_obj =
                            dynamic_cast<interval_matrix_object*>(obj);
                    sStream << interval_obj->getMatrix();
                    out << checkBox->text() << endl;
                    out << interval_obj->getRows() << " " << interval_obj->getColumns() << endl;
                    out << QString(sStream.str().c_str()) << endl;
                }
            }
        }
    }
    file.close();
    this->accept();
}
