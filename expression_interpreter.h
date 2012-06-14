#ifndef EXPRESSION_INTERPRETER_H
#define EXPRESSION_INTERPRETER_H

#include "scanner.h"
#include "object_storage.h"
#include "expr_except.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <cstddef>
#include "boost/numeric/interval.hpp"


/*!
  Пространство имен, содержащее классы ядра приложения.
 */
namespace int_calc
{

/*!
  \brief Класс, базовый для классов - узлов дерева выражения.
 */
class expression_node
{
public:
    /*!
      \brief Виртуальный метод вычисления результата узла дерева.
     */
    virtual stored_object* operator()() = 0;

    /*!
      \brief Пустой виртульный деструктор.
     */
    virtual ~expression_node() { }
};

/*!
  \brief Класс узла дерева, представляющий операцию унарного минуса.
 */
class unary_minus_node : public expression_node
{
public:
    /*!
      \brief Конструктор, создающий узел с заданным потомком.
      \param ch Указатель на дочерний узел.
     */
    unary_minus_node(expression_node *ch) :
        child(ch),
        result(0)
    {
    }
    virtual stored_object* operator()();
    virtual ~unary_minus_node();

private:
    expression_node *child; /*!< Указатель на потомка. */
    stored_object *result;  /*!< Результат вычисления узла. */
};

/*!
  \brief Класс узла дерева, представляющий бинарную операцию сложения.
 */
class plus_node : public expression_node
{
public:
    /*!
      \brief Конструктор, создающий узел с заданными потомками.
      \param l Указатель на левого потомка.
      \param r Указатель на правого потомка.
     */
    plus_node(expression_node *l,
              expression_node *r) :
        left(l),
        right(r),
        result(0)
    {
    }
    virtual stored_object* operator()();
    virtual ~plus_node();

private:
    expression_node *left;  /*!< Указатель на левого потомка. */
    expression_node *right; /*!< Указатель на правого потомка. */
    stored_object *result;  /*!< Результат вычисления узла. */
};

/*!
  \brief Класс узла дерева, представляющий бинарную операцию вычитания.
 */
class minus_node : public expression_node
{
public:
    /*!
      \brief Конструктор, создающий узел с заданными потомками.
      \param l Указатель на левого потомка.
      \param r Указатель на правого потомка.
     */
    minus_node(expression_node *l,
              expression_node *r) :
        left(l),
        right(r),
        result(0)
    {
    }
    virtual stored_object* operator()();
    virtual ~minus_node();

private:
    expression_node *left;  /*!< Указатель на левого потомка. */
    expression_node *right; /*!< Указатель на правого потомка. */
    stored_object *result;  /*!< Результат вычисления узла. */
};

/*!
  \brief Класс узла дерева, представляющий бинарную операцию умножения.
 */
class multiply_node : public expression_node
{
public:
    /*!
      \brief Конструктор, создающий узел с заданными потомками.
      \param l Указатель на левого потомка.
      \param r Указатель на правого потомка.
     */
    multiply_node(expression_node *l,
              expression_node *r) :
        left(l),
        right(r),
        result(0)
    {
    }
    virtual stored_object* operator()();
    virtual ~multiply_node();

private:
    expression_node *left;  /*!< Указатель на левого потомка. */
    expression_node *right; /*!< Указатель на правого потомка. */
    stored_object *result;  /*!< Результат вычисления узла. */
};

/*!
  \brief Класс узла дерева, представляющий бинарную операцию деления.
 */
class divide_node : public expression_node
{
public:
    /*!
      \brief Конструктор, создающий узел с заданными потомками.
      \param l Указатель на левого потомка.
      \param r Указатель на правого потомка.
     */
    divide_node(expression_node *l,
              expression_node *r) :
        left(l),
        right(r),
        result(0)
    {
    }
    virtual stored_object* operator()();
    virtual ~divide_node();

private:
    expression_node *left;  /*!< Указатель на левого потомка. */
    expression_node *right; /*!< Указатель на правого потомка. */
    stored_object *result;  /*!< Результат вычисления узла. */
};

/*!
  \brief Класс узла дерева, представляющий операцию создания интервала.
 */
class interval_creation_node : public expression_node
{
public:
    /*!
      \brief Конструктор, создающий узел с заданными потомками.
      \param l Указатель на левого потомка.
      \param r Указатель на правого потомка.
     */
    interval_creation_node(expression_node *l,
              expression_node *r) :
        left(l),
        right(r),
        result(0)
    {
    }
    virtual stored_object* operator ()();
    virtual ~interval_creation_node();

private:
    expression_node *left;  /*!< Указатель на левого потомка. */
    expression_node *right; /*!< Указатель на правого потомка. */
    stored_object *result;  /*!< Результат вычисления узла. */
};

/*!
  \brief Класс узла дерева, представляющий операцию создания матрицы.
 */
class matrix_creation_node : public expression_node
{
public:
    /*!
      \brief Конструктор по умолчанию.
     */
    matrix_creation_node() :
        result(0)
    {
    }

    /*!
      \brief Вектор, хранящий векторы указателей на потомков.
     */
    vector< vector<expression_node*> > childs;

    virtual stored_object* operator ()();
    virtual ~matrix_creation_node();

private:
    stored_object *result;  /*!< Результат вычисления узла. */
};

/*!
  \brief Класс узла дерева, представляющий операцию вызова функции.
 */
class function_call_node : public expression_node
{
public:
    vector<expression_node*> childs;    /*!< Вектор указателей на потомков.*/

    /*!
      \brief Конструктор, создающий узел вычисления функции f.
      \param f Указатель на объект, представляющий вычисляемую узлом функцию.
     */
    function_call_node(function_object *f) :
        func(f),
        result(0)
    {
    }
    virtual stored_object* operator ()();
    virtual ~function_call_node();

private:
    function_object *func;  /*!< Вычисляемая узлом функция. */
    stored_object *result;  /*!< Результат вычисления узла. */
};

/*!
  \brief Класс узла дерева, представляющий значение переменной.
 */
class variable_node : public expression_node
{
public:
    /*!
      \brief Конструктор, создающий узел с заданной переменной.
      \param var Указатель на хранимую переменную.
     */
    variable_node(stored_object *var) :
        variable(var)
    {
    }
    /*!
      Метод вычисления узла просто возвращает указатель на переменную.
     */
    virtual stored_object* operator()() { return variable; }
    /*!
      Пустой деструктор.
     */
    virtual ~variable_node() { }

private:
    stored_object *variable;    /*!< Указатель на переменную. */
};

/*!
  \brief Класс узла дерева, представляющий значение числовой константы.
 */
class constant_node : public expression_node
{
public:
    constant_node(double val);
    constant_node(interval_double ival);
    virtual stored_object* operator()();
    virtual ~constant_node();

private:
    stored_object *constant;    /*!< Указатель на объект, хранящий значение константы. */
};

/*!
  \brief Класс, представляющий функцию обращения к элементам
  или подматрицам матрицы.
 */
class appeal_elements : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

/*!
  \brief Класс, базовый для классов, представляющих результат интерпретации выражения.
 */
class expression_result
{
public:
    virtual ~expression_result() { }
};

/*!
  \brief Класс результата интерпретации, означающий,
  что произошло присвоение значения переменной.
 */
class assign_result : public expression_result
{
public:
    /*!
      \brief Конструктор, создающий объект с заданным именем переменной.
      \param var_name Имя переменной, которой присвоено значение.
     */
    assign_result(const std::string &var_name) :
        variable_name(var_name)
    {
    }

    /*!
      \return Имя переменной.
     */
    std::string getVarName() { return variable_name; }

private:
    std::string variable_name;  /*!< Имя переменной, которой присвоено значение. */
};

/*!
  \brief Класс результата интерпретации, означающий,
  что пользователь ввел служебную команду.
 */
class command_result : public expression_result
{
public:
    /*!
      \brief Конструктор, создающий объект с заданным именем команды.
      \param var_name Имя команды.
     */
    command_result(const std::string &comm_name) :
        command_name(comm_name)
    {
    }

    /*!
      \return Имя команды.
     */
    std::string getCommandName() { return command_name; }

private:
    std::string command_name;   /*!< Имя команды. */
};

/*!
  \brief Класс интерпретатора выражений.
 */
class expression_interpreter
{
public:
    expression_interpreter(object_storage *stor);
    expression_result* execute(std::string &cmd);

private:
    int_calc::scanner scan;     /*!< Сканер, используемый при анализе выражений. */
    object_storage *storage;    /*!< Хранилище объектов. */
    appeal_elements app_elem;   /*!< Функциональный объект для обращения к элементам матриц. */
    expression_result* result;  /*!< Результат интерпретации выражения. */

    expression_result* S();
    expression_result* C();
    expression_node* E();
    expression_node* U(expression_node *left_node, scanner::lexem_type op);
    expression_node* A(expression_node *left_node, scanner::lexem_type op);
    expression_node* M();
    void B(vector< vector<expression_node*> > &vect);
    void G(vector<expression_node*> &vect);
    void P(vector<expression_node*> &vect);
};

}

#endif // EXPRESSION_INTERPRETER_H
