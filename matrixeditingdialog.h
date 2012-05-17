#ifndef MATRIXEDITINGDIALOG_H
#define MATRIXEDITINGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "object_storage.h"

namespace Ui {
    class MatrixEditingDialog;
}

class MatrixEditingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatrixEditingDialog(QWidget *parent, int_calc::object_storage *st);
    ~MatrixEditingDialog();

    void setEditedMatrix(int_calc::matrix_object *matr);
    void setMatrixName(const QString &name);

private:
    Ui::MatrixEditingDialog *ui;

    int_calc::matrix_object *editedMatrix;
    int_calc::object_storage *stor;
    QString matrixName;
    int rowsCount;
    int columnsCount;
    QRegExp cellRegExp;

private slots:
    void changeRowsCount(int cnt);
    void changeColumnsCount(int cnt);
    bool checkCellContent(int row, int col);
    bool saveMatrix();

protected:
    void closeEvent(QCloseEvent *closeEvent);
};

#endif // MATRIXEDITINGDIALOG_H
