#include "systemmodelingdialog.h"
#include "ui_systemmodelingdialog.h"
#include <QtGui>
#include "qwt_series_data.h"

#include "numeric_methods.h"

using namespace int_calc;

SystemModelingDialog::SystemModelingDialog(int_calc::object_storage *stor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemModelingDialog),
    storage(stor),
    curve(0),
    numberRegExp("-?\\d+(\\.\\d+)?([eE][+-]\\d+)?")
{
    ui->setupUi(this);

    initStateLayout = new QVBoxLayout;
    initStateLayout->addStretch();
    ui->initStateGroupBox->setLayout(initStateLayout);


    qwtPlot = new QwtPlot();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(qwtPlot);
    ui->graphWidget->setLayout(mainLayout);
    qwtPlot->show();

    numberValidator = new QRegExpValidator(numberRegExp, this);

    ui->stepLineEdit->setValidator(numberValidator);
    ui->t0LineEdit->setValidator(numberValidator);
    ui->t1LineEdit->setValidator(numberValidator);

    //Соединение слотов с сигналами
    connect(ui->matrixComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(selectedMatrixChanged(QString)));
    connect(ui->startPushButton, SIGNAL(clicked()),
            this, SLOT(calculateFunction()));
    connect(ui->drawPushButton, SIGNAL(clicked()),
            this, SLOT(drawPushButton_clicked()));

    //
    QList<int> splitSisez;
    splitSisez.append(this->width() * 0.25);
    splitSisez.append(this->width() * 0.75);
    ui->splitter->setSizes(splitSisez);


    updateMatrixComboBox();
}

SystemModelingDialog::~SystemModelingDialog()
{
    delete ui;
}

void SystemModelingDialog::updateMatrixComboBox()
{
    ui->matrixComboBox->clear();
    ui->matrixComboBox->addItem("");
    for (named_object obj = storage->getFirst(); obj.getObject(); obj = storage->getNext())
    {
        if (numeric_matrix_object *matr_obj =
                dynamic_cast<numeric_matrix_object*>(obj.getObject()))
        {
            if (matr_obj->getRows() == matr_obj->getColumns())
            {
                ui->matrixComboBox->addItem(QString(obj.getName().c_str()));
            }
        }
    }
}

void SystemModelingDialog::changeMatrix(std::string name)
{
    ui->rightWidget->setEnabled(false);

    if (name.empty())
    {
        for (int i = 0; i != initStateLineEdits.size(); ++i)
        {
            initStateLayout->removeWidget(initStateLineEdits[i]);
            delete initStateLineEdits[i];
        }
        initStateLineEdits.clear();

        for (int i = 0; i != initStateLabels.size(); ++i)
        {
            initStateLayout->removeWidget(initStateLabels[i]);
            delete initStateLabels[i];
        }
        initStateLabels.clear();

        ui->startPushButton->setEnabled(false);

        return;
    }

    ui->startPushButton->setEnabled(true);

    stored_object *obj = storage->getObjectByName(name);
    if (!obj)
    {
        return;
    }

    matrix_object *matr_obj =
            dynamic_cast<matrix_object*>(obj);

    int n = matr_obj->getRows();
    int oldSize = initStateLineEdits.size();
    QLineEdit *lineEdit = 0;
    QLabel *label = 0;
    if (n > oldSize)
    {
        for (int i = 0; i < n - oldSize; ++i)
        {
            label = new QLabel(QString("x") + QString::number(oldSize + i + 1));
            initStateLayout->insertWidget(initStateLayout->count() - 1, label);
            initStateLabels.push_back(label);
            label->show();

            lineEdit = new QLineEdit;
            initStateLayout->insertWidget(initStateLayout->count() - 1, lineEdit);
            initStateLineEdits.push_back(lineEdit);
            lineEdit->setValidator(numberValidator);
            lineEdit->show();
        }
    }
    else if (n < oldSize)
    {
        for (int i = 0; i < oldSize - n; ++i)
        {
            lineEdit = initStateLineEdits[initStateLineEdits.size() - 1];
            initStateLineEdits.remove(initStateLineEdits.size() - 1);
            delete lineEdit;

            label = initStateLabels[initStateLabels.size() - 1];
            initStateLabels.remove(initStateLabels.size() - 1);
            delete label;
        }
    }

    ui->coordComboBox->clear();
    for (int i = 0; i < n; ++i)
    {
        ui->coordComboBox->addItem(QString::number(i + 1));
    }
}

void SystemModelingDialog::selectedMatrixChanged(QString name)
{
    changeMatrix(name.toStdString());
}

void SystemModelingDialog::drawPushButton_clicked()
{
   int coord = ui->coordComboBox->currentIndex();
   drawGraph(coord);
}

bool SystemModelingDialog::checkInput()
{
    if(!ui->stepLineEdit->hasAcceptableInput())
    {
        ui->stepLineEdit->setFocus();
        return false;
    }
    if(!ui->t0LineEdit->hasAcceptableInput())
    {
        ui->t0LineEdit->setFocus();
        return false;
    }
    if(!ui->t1LineEdit->hasAcceptableInput())
    {
        ui->t1LineEdit->setFocus();
        return false;
    }

    for (int i = 0; i != initStateLineEdits.size(); ++i)
    {
        if(!initStateLineEdits[i]->hasAcceptableInput())
        {
            initStateLineEdits[i]->setFocus();
            return false;
        }
    }

    return true;
}

void SystemModelingDialog::calculateFunction()
{
    std::string name = ui->matrixComboBox->currentText().toStdString();
    if (name.empty())
    {
        return;
    }

    if (!checkInput())
    {
        QMessageBox::warning(this, "", trUtf8("Нужно ввести число"));
        return;
    }

    stored_object *obj = storage->getObjectByName(name);
    numeric_matrix_object *num_obj =
            dynamic_cast<numeric_matrix_object*>(obj);

    int n = num_obj->getRows();
    matrix<double> x0(n, 1);
    for (int i = 0; i != initStateLineEdits.size(); ++i)
    {
        x0(i, 0) = initStateLineEdits[i]->text().toDouble();
    }

    h = ui->stepLineEdit->text().toDouble();
    t0 = ui->t0LineEdit->text().toDouble();
    t1 = ui->t1LineEdit->text().toDouble();

    if (t0 > t1)
    {
        QMessageBox::warning(this, "", trUtf8("t0 должно быть меньше t1"));
        ui->t0LineEdit->setFocus();
        return;
    }

    func.clear();
    num_methods::numericEuler(num_obj->getMatrix(), x0, h, t0, t1, func);

    drawGraph(0);
    ui->rightWidget->setEnabled(true);
    ui->coordComboBox->setCurrentIndex(0);
}

void SystemModelingDialog::drawGraph(int coord)
{
    QVector<double> dataX, dataY;
    for (int i = 0; i < func.size(); ++i)
    {
        dataX.push_back(t0 + i * h);
        dataY.push_back(func[i](coord, 0));
    }

    QwtPointArrayData *data = new QwtPointArrayData(dataX, dataY);

    delete curve;
    curve = new QwtPlotCurve();
    curve->setData(data);

    curve->attach(qwtPlot);

    qwtPlot->replot();
}
