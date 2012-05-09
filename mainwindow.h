#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "object_storage.h"
#include "expression_interpreter.h"

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
    object_storage *storage;
    expression_interpreter *interpreter;
    QStringList commands;
    int currentCommandIndex;

private slots:
    void executeCommand();

protected:
    void keyPressEvent(QKeyEvent *keyEvent);


};

#endif // MAINWINDOW_H
