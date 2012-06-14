/*!
  \file object_storage.h

  В этом файле определяется абстрактный контейнерный класс object_storage,
  классы объектов, которые могут храниться в object_storage, и некоторые функции
  от этих объектов.
 */

#ifndef OBJECT_STORAGE_H
#define OBJECT_STORAGE_H

#include <string>
#include <map>
#include <vector>

#include "matrix.h"
#include "expr_except.h"
#include "boost/numeric/interval.hpp"

using namespace boost::numeric::interval_lib;

/*!
  Тип политики округления, применяемый в экземпляре шаблонного класса интервала,
  используемого в приложении.
 */
typedef policies<save_state<rounded_transc_opp<double> >,
                            checking_base<double> > defpolicy;

/*!
  Экземпляр шаблонного класса интервала, используемый для представления
  числового интервала в данном приложении
 */
typedef boost::numeric::interval<double, defpolicy> interval_double;

/*!
  Пространство имен, содержащее классы ядра приложения.
 */
namespace int_calc
{

/*!
 Класс, базовый для всех классов объектов, представляющих
 сущности, к которым можно обращаться по имени в программе и
 которые могут храниться в контейнере object_storage.
 \sa object_storage
 */
class stored_object
{
public:
    /*!
      \brief Пустой виртуальный деструктор.
     */
    virtual ~stored_object() { }

    /*!
      \brief Виртуальный метод для создания копии объекта.
     */
    virtual stored_object* copy() = 0;
};

/*!
  Класс, инкапсулирующий указатель на stored_object
  и строку с именем объекта. Используется для обхода
  контейнера object_storage.
  \sa object_stotage
 */
class named_object
{
public:
    /*!
      Конструктор, инициализирующий все поля объекта своими параметрами.

      \param nam Имя объекта.
      \param obj Инкапсулируемый указатель на объект.
     */
    named_object(const std::string &nam, stored_object *obj) :
        name(nam), object(obj)
    {
    }

    /*!
      \return Имя объекта.
     */
    const std::string& getName() { return name; }

    /*!
      \return Инкапсулируемый указатель на объект.
     */
    stored_object* getObject() { return object; }

private:
    std::string name;   /*!< Имя объекта. */
    stored_object *object;  /*!< Инкапсулируемый указатель на объект. */
};

/*!
  Класс, базовый для классов, служащих оболочкой для матричных значений.
 */
class matrix_object : public stored_object
{
public:
    virtual ~matrix_object() { }

    /*!
      \return Количество строк инкапсулируемой матрицы.
     */
    virtual size_t getRows() = 0;

    /*!
      \return Количество столбцов инкапсулируемой матрицы.
     */
    virtual size_t getColumns() = 0;

    virtual stored_object* copy() = 0;
};

/*!
  Класс, являющийся оболочкой для числовой матрицы.
 */
class numeric_matrix_object : public matrix_object
{
public:
    /*!
      \param matr Инкапсулируемая числовая матрица.
     */
    numeric_matrix_object(matrix<double> &matr) :
        matrix_variable(matr)
    {
    }

    /*!
      Конструктор, создающий инкапсулируемую матрицу с
      заданным количеством строк и столбцов.
      \param r Количество строк инкапсулируемой матрицы.
      \param c Количество столбцов инкапсулируемой матрицы.
     */
    numeric_matrix_object(size_t r, size_t c) :
        matrix_variable(r, c)
    {
    }

    /*!
      Возвращает неконстантную ссылку на матрицу, позволяя
      использовать вызов метода в левой части оператора присвоения.
      \return Инкапсулируемая числовая матрица.
     */
    matrix<double>& getMatrix() { return matrix_variable; }

    virtual size_t getRows() { return matrix_variable.getRows(); }
    virtual size_t getColumns() { return matrix_variable.getColumns(); }
    virtual stored_object* copy();
    virtual ~numeric_matrix_object() { }

private:
    matrix<double> matrix_variable; /*!< Инкапсулируемая числовая матрица*/
};

/*!
  Класс, являющийся оболочкой для интервальной матрицы.
 */
class interval_matrix_object : public matrix_object
{
public:
    /*!
      \param matr Инкапсулируемая интервальная матрица.
     */
    interval_matrix_object(matrix< interval_double > &matr) :
        matrix_variable(matr)
    {
    }

    /*!
      Конструктор, создающий инкапсулируемую матрицу с
      заданным количеством строк и столбцов.
      \param r Количество строк инкапсулируемой матрицы.
      \param c Количество столбцов инкапсулируемой матрицы.
     */
    interval_matrix_object(size_t r, size_t c) :
        matrix_variable(r, c)
    {
    }

    /*!
      Возвращает неконстантную ссылку на матрицу, позволяя
      использовать вызов метода в левой части оператора присвоения.
      \return Инкапсулируемая интервальная матрица.
     */
    matrix< interval_double >& getMatrix() { return matrix_variable; }

    virtual size_t getRows() { return matrix_variable.getRows(); }
    virtual size_t getColumns() { return matrix_variable.getColumns(); }
    virtual ~interval_matrix_object() { }
    virtual stored_object* copy();

private:
    matrix< interval_double >
    matrix_variable;  /*!< Инкапсулируемая интервальная матрица.*/
};

/*!
  Абстрактный класс, базовый для классов, представляющих функции,
  доступные для вызова пользователем в командной строке.
 */
class function_object : public stored_object
{
public:
    /*!
      Метод, вычисляющий представляемую классом функцию.
     */
    virtual stored_object* operator ()(std::vector<stored_object*> &args) = 0;
    virtual ~function_object() { }

    /*!
      Наследуемый метод копирования возвращает нулевой указатель,
      т.к. предполагается, что наследники класса будут создаваться
      в единственном экземпляре.
     */
    virtual stored_object* copy() { return 0; }
};


/*!
  Абстрактный класс, задающий интерфейс контейнера для указателей
  на объекты, к которым можно будет обращаться по имени в выражениях,
  вводимых в командной строке пользователем.
 */
class object_storage
{
public:
    /*!
      \brief Добавляет объект с заданным именем в контейнер.
      \param nam Имя добавляемого объекта.
      \param obj Добавляемый указатель на объект.
     */
    virtual void addObject(const std::string &nam, stored_object *obj) = 0;

    /*!
      \brief Удаляет из контейнера объект с заданным именем.
      \param nam Имя удаляемого объекта.
     */
    virtual void deleteObject(const std::string &nam) = 0;

    /*!
      \brief Возвращает указатель на объект с заданным именем.
      \param nam Имя искомого объекта.
      \return Указатель на объект с заданным именем, либо нулевой указатель,
      если объект не найден.
     */
    virtual stored_object* getObjectByName(const std::string &nam) = 0;

    /*!
      \brief Удаляет все объекты из контейнера
     */
    virtual void clear() = 0;

    /*!
      \brief Начинает обход контейнера и возвращает указатель на первый объект
      вместе с его именем.
      \return Объект класса named_object, содержащий указатель на первый объект
      контейнера, и имя объекта. Если контейнер пуст, указатель на объект будет
      нулевым.
     */
    virtual named_object getFirst() = 0;

    /*!
      \brief Переход к следующему объекту контейнера.
      \return Объект класса named_object, содержащий указатель на очередной объект
      контейнера, и имя объекта. Если все объекты контейнера пройдены, указатель
      на объект будет нулевым.
     */
    virtual named_object getNext() = 0;

    /*!
      \brief Пустой виртуальный деструктор.
     */
    virtual ~object_storage() { }

    object_storage() { }
};

/*!
  Класс, реализующий интерфейс контейнера object_storage и
  хранящий объекты в ассоциативном контейнере std::map.
 */
class map_object_storage : public object_storage
{
public:
    virtual void addObject(const std::string &nam, stored_object *obj);
    virtual void deleteObject(const std::string &nam);
    virtual stored_object* getObjectByName(const std::string &nam);
    virtual void clear();

    virtual named_object getFirst();
    virtual named_object getNext();

    virtual ~map_object_storage() { destroy(); }


private:
    /*!
      \brief Тип контейнера для хранения объектов.
     */
    typedef std::map<std::string, stored_object*> storage_type;

    /*!
      \brief Хранилище объектов.
     */
    std::map<std::string, stored_object*> storage;

    /*!
      \brief Итератор, указывающий на текущий объект при обходе контейнера.
      \sa getFirst()
      \sa getNext()
     */
    std::map<std::string, stored_object*>::iterator cur_object;

    void destroy();
};


//*****************************************************************************
matrix_object& operator +(matrix_object &lhs, matrix_object &rhs);
matrix_object& operator -(matrix_object &lhs, matrix_object &rhs);
matrix_object& operator *(matrix_object &lhs, matrix_object &rhs);
matrix_object& operator /(matrix_object &lhs, matrix_object &rhs);

matrix_object& elementwiseProduct(matrix_object &lhs, matrix_object &rhs);
matrix_object& kronekerProduct(matrix_object &lhs, matrix_object &rhs);

interval_matrix_object* convertNumericToInterval(numeric_matrix_object *num_obj);

/*!
  \brief Применяет заданную функцию к элементам матрицы.
  \tparam T Тип элемента матрицы.
  \param matr Матрица, к элементам которой нужно применить функцию.
  \param func Указатель на функцию, применяемую к элементам матрицы.
 */
template <typename T> void applyToElements(matrix<T>& matr, T(*func)(T))
{
    for(size_t i = 0; i != matr.getRows(); ++i)
    {
        for(size_t j = 0; j != matr.getColumns(); ++j)
        {
            matr(i, j) = func(matr(i, j));
        }
    }
}

/*!
  \brief Применяет заданную функцию, принимающую константный аргумент,
  к элементам матрицы.
  \tparam T Тип элемента матрицы.
  \param matr Матрица, к элементам которой нужно применить функцию.
  \param func Указатель на функцию, применяемую к элементам матрицы.
 */
template <typename T> void applyToConstElements(matrix<T>& matr, T(*func)(const T&))
{
    for(size_t i = 0; i != matr.getRows(); ++i)
    {
        for(size_t j = 0; j != matr.getColumns(); ++j)
        {
            matr(i, j) = func(matr(i, j));
        }
    }
}
//*****************************************************************************

}


#endif // OBJECT_STORAGE_H
