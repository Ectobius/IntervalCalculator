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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MatrixEditingDialog *matrixEditingDialog;
    object_storage *storage;
    expression_interpreter *interpreter;
    QStringList commands;
    int currentCommandIndex;

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
