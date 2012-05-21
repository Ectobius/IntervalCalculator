#include "systemmodelingdialog.h"
#include "ui_systemmodelingdialog.h"
#include <QtGui>
#include "qwt_plot_curve.h"
#include "qwt_series_data.h"
#include <vector>

#include "numeric_methods.h"

using namespace int_calc;

SystemModelingDialog::SystemModelingDialog(int_calc::object_storage *stor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemModelingDialog),
    storage(stor)
{
    ui->setupUi(this);

    initStateLayout = new QVBoxLayout;
    ui->initStateGroupBox->setLayout(initStateLayout);

    qwtPlot = new QwtPlot();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(qwtPlot);
    ui->graphWidget->setLayout(mainLayout);
    qwtPlot->show();

    //Соединение слотов с сигналами
    connect(ui->matrixComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(selectedMatrixChanged(QString)));
    connect(ui->startPushButton, SIGNAL(clicked()),
            this, SLOT(drawGraph()));

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
        if (matrix_object *matr_obj =
                dynamic_cast<matrix_object*>(obj.getObject()))
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
    if (name.empty())
    {
        QMessageBox::warning(this, "", "Yes");
        for (int i = 0; i != initStateLineEdits.size(); ++i)
        {
            initStateLayout->removeWidget(initStateLineEdits[i]);
            delete initStateLineEdits[i];
        }
        initStateLineEdits.clear();
        return;
    }

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
    if (n > oldSize)
    {
        for (int i = 0; i < n - oldSize; ++i)
        {
            lineEdit = new QLineEdit;
            initStateLayout->addWidget(lineEdit);
            initStateLineEdits.push_back(lineEdit);
            lineEdit->show();
        }
    }
    else if (n < oldSize)
    {
        for (int i = 0; i < oldSize - n; ++i)
        {
            initStateLineEdits.remove(initStateLineEdits.size() - 1);
        }
    }
}

void SystemModelingDialog::selectedMatrixChanged(QString name)
{
    changeMatrix(name.toStdString());
}

void SystemModelingDialog::drawGraph()
{
    std::string name = ui->matrixComboBox->currentText().toStdString();
    if (name.empty())
    {
        return;
    }

    stored_object *obj = storage->getObjectByName(name);
    interval_matrix_object *interval_obj = 0;
    if (dynamic_cast<interval_matrix_object*>(obj))
    {
        interval_obj = dynamic_cast<interval_matrix_object*>(obj);
    }
    else if(numeric_matrix_object *num_obj =
            dynamic_cast<numeric_matrix_object*>(obj))
    {
        interval_obj = convertNumericToInterval(num_obj);
    }

    int n = interval_obj->getRows();
    matrix<d_interval> x0(n, 1);
    for (int i = 0; i != initStateLineEdits.size(); ++i)
    {
        x0(i, 0) = initStateLineEdits[i]->text().toDouble();
    }

    double h = ui->stepLineEdit->text().toDouble();
    double t0 = ui->t0LineEdit->text().toDouble();
    double t1 = ui->t1LineEdit->text().toDouble();

    vector< matrix<d_interval> > func;
    num_methods::intervalEuler(interval_obj->getMatrix(), x0, h, t0, t1, func);

    QVector<double> dataX, dataY;
    for (int i = 0; i < func.size(); ++i)
    {
        dataX.push_back(t0 + i * h);
        dataY.push_back(func[i](0, 0).upper());
    }

    QwtPointArrayData *data = new QwtPointArrayData(dataX, dataY);

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setData(data);

    curve->attach(qwtPlot);
    curve->show();

    qwtPlot->replot();
}
