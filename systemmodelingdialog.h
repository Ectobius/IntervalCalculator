#ifndef SYSTEMMODELINGDIALOG_H
#define SYSTEMMODELINGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "qwt_plot.h"
#include "object_storage.h"
#include <string>
#include <QVector>

class QVBoxLayout;

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
    QVBoxLayout *initStateLayout;

    void changeMatrix(std::string name);

public slots:
    void updateMatrixComboBox();
    void selectedMatrixChanged(QString name);

    void drawGraph();
};

#endif // SYSTEMMODELINGDIALOG_H
