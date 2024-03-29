#ifndef MATRIXSAVINGDIALOG_H
#define MATRIXSAVINGDIALOG_H

#include <QDialog>
#include "object_storage.h"

namespace Ui {
    class MatrixSavingDialog;
}

/*!
  \brief Класс, представляющий окно сохранения матриц в файл.
 */
class MatrixSavingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatrixSavingDialog(int_calc::object_storage *stor, QWidget *parent = 0);
    ~MatrixSavingDialog();

private:
    Ui::MatrixSavingDialog *ui;

    int_calc::object_storage *storage;  /*!< Хранилище объектов. */

private slots:
    void loadVariables();
    void setListCheckState(bool state);
    void selectAllVariables();
    void cancelAllVariables();
    void selectFile();
    void saveVariables();
};

#endif // MATRIXSAVINGDIALOG_H
