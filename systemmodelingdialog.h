#ifndef SYSTEMMODELINGDIALOG_H
#define SYSTEMMODELINGDIALOG_H

#include <QDialog>
#include "qwt_plot.h"
#include "object_storage.h"
#include <string>
#include <QVector>
#include <vector>
#include "qwt_plot_curve.h"

class QVBoxLayout;
class QRegExpValidator;
class QLineEdit;
class QLabel;

namespace Ui {
    class SystemModelingDialog;
}

class SystemModelingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemModelingDialog(int_calc::object_storage *stor, QWidget *parent = 0);
    ~SystemModelingDialog();

private:
    Ui::SystemModelingDialog *ui;
    int_calc::object_storage *storage;
    QwtPlot *qwtPlot;
    QVector<QLineEdit*> initStateLineEdits;
    QVector<QLabel*> initStateLabels;
    QVBoxLayout *initStateLayout;
    QwtPlotCurve *curve;
    std::vector< int_calc::matrix<double> > func;
    QRegExp numberRegExp;
    QRegExpValidator *numberValidator;

    double t0;
    double t1;
    double h;

    void changeMatrix(std::string name);

public slots:
    void updateMatrixComboBox();
    void selectedMatrixChanged(QString name);
    void drawPushButton_clicked();

    void calculateFunction();
    bool checkInput();

    void drawGraph(int coord);
};

#endif // SYSTEMMODELINGDIALOG_H
