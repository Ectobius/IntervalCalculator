#ifndef MATRIXEDITINGDIALOG_H
#define MATRIXEDITINGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "object_storage.h"

namespace Ui {
    class MatrixEditingDialog;
}

/*!
  \brief Класс, представляющий окно редактирования матрицы.
 */
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
    int_calc::matrix_object *editedMatrix;  /*!< Редактируемая матрица. */
    int_calc::object_storage *stor; /*!< Хранилище переменных. */
    QString matrixName;             /*!< Имя редактируемой матрицы. */
    int rowsCount;                  /*!< Количество строк таблицы. */
    int columnsCount;               /*!< Количество столбцов таблицы. */
    QRegExp cellRegExp;             /*!< Регулярное выражение для проверки текста в ячейке таблицы. */

private slots:
    void changeRowsCount(int cnt);
    void changeColumnsCount(int cnt);
    bool checkCellContent(int row, int col);
    bool saveMatrix();

protected:
    void closeEvent(QCloseEvent *closeEvent);
};

#endif // MATRIXEDITINGDIALOG_H
