#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTextStream>
#include <QList>
#include "object_storage.h"
#include "expression_interpreter.h"
#include "matrixeditingdialog.h"

using namespace int_calc;


namespace Ui {
    class MainWindow;
}

/*!
  \brief Класс, представляющий главное окно приложения.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MatrixEditingDialog *matrixEditingDialog;   /*!< Окно редактирования матрицы. */
    object_storage *storage;                    /*!< Хранилище переменных. */
    expression_interpreter *interpreter;        /*!< Интерпретатор выражений. */
    QStringList commands;                       /*!< Список введенных команд. */
    int currentCommandIndex;                    /*!< Индекс текущей команды. */

    void loadVariablesToList(QList<named_object> &lst, QTextStream &inStream);

private slots:
    void executeCommand();
    void updateVariableList();
    void editMatrix(QTreeWidgetItem *item);
    void addMatrix();
    void deleteMatrix();
    void saveVariables();
    void loadVariables();
    void modelSystem();
    void clearConsole();
    void deleteVariables();

    void about();
    void aboutQt();

protected:
    void keyPressEvent(QKeyEvent *keyEvent);


};

#endif // MAINWINDOW_H
