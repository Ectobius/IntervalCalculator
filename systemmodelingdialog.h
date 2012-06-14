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

/*!
  \brief Класс окна моделирования систем дифференциальных уравнений.
 */
class SystemModelingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SystemModelingDialog(int_calc::object_storage *stor, QWidget *parent = 0);
    ~SystemModelingDialog();

private:
    Ui::SystemModelingDialog *ui;
    int_calc::object_storage *storage;      /*!< Хранилище переменных. */
    QwtPlot *qwtPlot;                       /*!< Виджет для рисования графика. */
    QVector<QLineEdit*> initStateLineEdits; /*!< Контейнер полей для ввода начального состояния. */
    QVector<QLabel*> initStateLabels;       /*!< Контейнер лэйблов полей начального состояния. */
    QVBoxLayout *initStateLayout;           /*!< Менеджер компоновки для виджетов ввода начального состояния. */
    QwtPlotCurve *curve;                    /*!< Рисуемая кривая. */
    std::vector< int_calc::matrix<double> > func;   /*!< Вектор значений функции. */
    QRegExp numberRegExp;               /*!< Регулярное выражение для десятичного числа. */
    QRegExpValidator *numberValidator;  /*!< Валидатор для проверки на число. */

    double t0;  /*!< Начальное значение переменной. */
    double t1;  /*!< Конечное значение переменной. */
    double h;   /*!< Шаг моделирования. */

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
