#ifndef OBJECT_STORAGE_CPP
#define OBJECT_STORAGE_CPP

#include "object_storage.h"

#include <algorithm>

using boost::numeric::interval;

namespace int_calc
{

stored_object* numeric_matrix_object::copy()
{
    numeric_matrix_object *obj = new numeric_matrix_object(*this);
    return obj;
}

stored_object* interval_matrix_object::copy()
{
    interval_matrix_object *obj = new interval_matrix_object(*this);
    return obj;
}

void map_object_storage::addObject(const std::string &nam, stored_object *obj)
{
    storage_type::iterator iter = storage.find(nam);
    if(iter == storage.end())
    {
        storage[nam] = obj;
    }
    else
    {
        delete iter->second;
        iter->second = obj;
    }
}

void map_object_storage::deleteObject(const std::string &nam)
{
    storage_type::iterator iter = storage.find(nam);
    storage.erase(iter);
}

stored_object* map_object_storage::getObjectByName(const std::string &nam)
{
    storage_type::iterator iter = storage.find(nam);
    if(iter != storage.end())
    {
        return iter->second;
    }
    else
    {
        return 0;
    }
}

void map_object_storage::clear()
{
    storage.clear();
}

named_object map_object_storage::getFirst()
{
    cur_object = storage.begin();
    if(cur_object != storage.end())
    {
        return named_object(cur_object->first, cur_object->second);
    }
    else
    {
        stored_object *nullObj = 0;
        std::string str;
        return named_object(std::string(), 0);
    }
}

named_object  map_object_storage::getNext()
{
    ++cur_object;
    if(cur_object != storage.end())
    {
        return named_object(cur_object->first, cur_object->second);
    }
    else
    {
        stored_object *nullObj = storage.begin()->second;
        std::string str;
        return named_object(std::string(), 0);
    }
}

void map_object_storage::destroy()
{
    for(storage_type::iterator iter = storage.begin(); iter != storage.end(); ++iter)
    {
        delete iter->second;
    }
}

//*************************************************************************************

/*!
  \brief Перегруженный оператор суммы объектов класса matrix_object.

  Осуществляет сложение матриц, заключенных в объектах класса
  matrix_object. Может вызываться только для объектов классов
  numeric_matrix_object и interval_matrix_object.
  Если одна из матриц размера 1x1, рассматривает ее как скаляр и
  прибавляет единственный элемент этой матрицы ко всем элементам
  другого операнда.

  \param lhs Левый операнд.
  \param rhs Правый операнд.
  \return Объект, содержащий сумму операндов.
 */
matrix_object& operator +(matrix_object &lhs, matrix_object &rhs)
{
    matrix_object *result = 0;

    if(dynamic_cast<numeric_matrix_object*>(&lhs))
    {
        numeric_matrix_object *left_num =
                dynamic_cast<numeric_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            numeric_matrix_object *num_res = 0;
            if(left_num->getRows() == 1 && left_num->getColumns() == 1)
            {
                num_res = new numeric_matrix_object(right_num->getMatrix());
                num_res->getMatrix() += left_num->getMatrix()(0, 0);
            }
            else
            {
                num_res = new numeric_matrix_object(left_num->getMatrix());
                if(right_num->getRows() == 1 && right_num->getColumns() == 1)
                {
                    num_res->getMatrix() += right_num->getMatrix()(0, 0);
                }
                else
                {
                    num_res->getMatrix() += right_num->getMatrix();
                }
            }
            result = num_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);

            if(left_num->getRows() == 1 && left_num->getColumns() == 1)
            {
                interval_res->getMatrix().assign(right_interval->getMatrix());
                interval_res->getMatrix() += left_num->getMatrix()(0, 0);
            }
            else
            {
                interval_res->getMatrix().assign(left_num->getMatrix());
                if(right_interval->getRows() == 1 && right_interval->getColumns() == 1)
                {
                    interval_res->getMatrix() += right_interval->getMatrix()(0, 0);
                }
                else
                {
                    interval_res->getMatrix() += right_interval->getMatrix();
                }
            }
            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else if(dynamic_cast<interval_matrix_object*>(&lhs))
    {
        interval_matrix_object *left_interval =
                dynamic_cast<interval_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            interval_matrix_object *interval_res = 0;
            if(left_interval->getRows() == 1 && left_interval->getColumns() == 1)
            {
                interval_res = new interval_matrix_object(0, 0);
                interval_res->getMatrix().assign(right_num->getMatrix());
                interval_res->getMatrix() += left_interval->getMatrix()(0, 0);
            }
            else
            {
                interval_res = new interval_matrix_object(left_interval->getMatrix());
                if(right_num->getRows() == 1 && right_num->getColumns() == 1)
                {
                    interval_res->getMatrix() += right_num->getMatrix()(0, 0);
                }
                else
                {
                    interval_res->getMatrix() += right_num->getMatrix();
                }
            }
            result = interval_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);

            if(left_interval->getRows() == 1 && left_interval->getColumns() == 1)
            {
                interval_res->getMatrix().assign(right_interval->getMatrix());
                interval_res->getMatrix() += left_interval->getMatrix()(0, 0);
            }
            else
            {
                interval_res->getMatrix().assign(left_interval->getMatrix());
                if(right_interval->getRows() == 1 && right_interval->getColumns() == 1)
                {
                    interval_res->getMatrix() += right_interval->getMatrix()(0, 0);
                }
                else
                {
                    interval_res->getMatrix() += right_interval->getMatrix();
                }
            }
            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else
    {
        throw runtime_error("Unknown type");
    }

    return *result;
}

/*!
  \brief Перегруженный оператор разности объектов класса matrix_object.

  Осуществляет вычитание матриц, заключенных в объектах класса
  matrix_object. Может вызываться только для объектов классов
  numeric_matrix_object и interval_matrix_object.
  Если матрица в правом операнде имеет размер 1x1, рассматривает ее
  как скаляр и вычитает ее единственный элемент из всех элементов
  левого операнда.

  \param lhs Левый операнд.
  \param rhs Правый операнд.
  \return Объект, содержащий разность операндов.
 */
matrix_object& operator -(matrix_object &lhs, matrix_object &rhs)
{
    matrix_object *result = 0;

    if(dynamic_cast<numeric_matrix_object*>(&lhs))
    {
        numeric_matrix_object *left_num =
                dynamic_cast<numeric_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            numeric_matrix_object *num_res = 0;
            if(left_num->getRows() == 1 && left_num->getColumns() == 1)
            {
                num_res = new numeric_matrix_object(right_num->getMatrix());
                num_res->getMatrix() *= -1;
                num_res->getMatrix() += left_num->getMatrix()(0, 0);
            }
            else
            {
                num_res = new numeric_matrix_object(left_num->getMatrix());
                if(right_num->getRows() == 1 && right_num->getColumns() == 1)
                {
                    num_res->getMatrix() -= right_num->getMatrix()(0, 0);
                }
                else
                {
                    num_res->getMatrix() -= right_num->getMatrix();
                }
            }
            result = num_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);            
            if(left_num->getRows() == 1 && left_num->getColumns() == 1)
            {
                interval_res->getMatrix().assign(right_interval->getMatrix());
                interval_res->getMatrix() *= -1;
                interval_res->getMatrix() += left_num->getMatrix()(0, 0);
            }
            else
            {
                interval_res->getMatrix().assign(left_num->getMatrix());
                if(right_interval->getRows() == 1 && right_interval->getColumns() == 1)
                {
                    interval_res->getMatrix() -= right_interval->getMatrix()(0, 0);
                }
                else
                {
                    interval_res->getMatrix() -= right_interval->getMatrix();
                }
            }
            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else if(dynamic_cast<interval_matrix_object*>(&lhs))
    {
        interval_matrix_object *left_interval =
                dynamic_cast<interval_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            interval_matrix_object *interval_res = 0;
            if(left_interval->getRows() == 1 && left_interval->getColumns() == 1)
            {
                interval_res = new interval_matrix_object(0, 0);
                interval_res->getMatrix().assign(right_num->getMatrix());
                interval_res->getMatrix() *= -1;
                interval_res->getMatrix() += left_interval->getMatrix()(0, 0);
            }
            else
            {
                interval_res = new interval_matrix_object(left_interval->getMatrix());
                if(right_num->getRows() == 1 && right_num->getColumns() == 1)
                {
                    interval_res->getMatrix() -= right_num->getMatrix()(0, 0);
                }
                else
                {
                    interval_res->getMatrix() -= right_num->getMatrix();
                }
            }
            result = interval_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);
            if(left_interval->getRows() == 1 && left_interval->getColumns() == 1)
            {
                interval_res->getMatrix().assign(right_interval->getMatrix());
                interval_res->getMatrix() *= -1;
                interval_res->getMatrix() += left_interval->getMatrix()(0, 0);
            }
            else
            {
                interval_res->getMatrix().assign(left_interval->getMatrix());
                if(right_interval->getRows() == 1 && right_interval->getColumns() == 1)
                {
                    interval_res->getMatrix() -= right_interval->getMatrix()(0, 0);
                }
                else
                {
                    interval_res->getMatrix() -= right_interval->getMatrix();
                }
            }
            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else
    {
        throw runtime_error("Unknown type");
    }

    return *result;
}

/*!
  \brief Перегруженный оператор умножения объектов класса matrix_object.

  Осуществляет умножение матриц, заключенных в объектах класса
  matrix_object. Может вызываться только для объектов классов
  numeric_matrix_object и interval_matrix_object.

  \param lhs Левый операнд.
  \param rhs Правый операнд.
  \return Объект, содержащий произведение операндов.
 */
matrix_object& operator *(matrix_object &lhs, matrix_object &rhs)
{
    matrix_object *result = 0;

    if(dynamic_cast<numeric_matrix_object*>(&lhs))
    {
        numeric_matrix_object *left_num =
                dynamic_cast<numeric_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            numeric_matrix_object *num_res =
                    new numeric_matrix_object(0, 0);

            matrix<double>::multiply(num_res->getMatrix(),
                                     left_num->getMatrix(), right_num->getMatrix());

            result = num_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);

            matrix< interval_double >::multiply(interval_res->getMatrix(),
                                     left_num->getMatrix(), right_interval->getMatrix());

            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else if(dynamic_cast<interval_matrix_object*>(&lhs))
    {
        interval_matrix_object *left_interval =
                dynamic_cast<interval_matrix_object*>(&lhs);
        interval_matrix_object *interval_res =
                new interval_matrix_object(0, 0);

        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            matrix< interval_double >::multiply(interval_res->getMatrix(),
                                     left_interval->getMatrix(), right_num->getMatrix());
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            matrix< interval_double >::multiply(interval_res->getMatrix(),
                                     left_interval->getMatrix(), right_interval->getMatrix());
        }
        else
        {
            throw runtime_error("Unknown type");
        }
        result = interval_res;
    }
    else
    {
        throw runtime_error("Unknown type");
    }

    return *result;
}

/*!
  \brief Перегруженный оператор деления объектов класса matrix_object.

  Требует, чтобы правый операнд содержал матрицу размера 1x1.
  Осуществляет деление элементов матрицы правого операнда на элемент матрицы левого.
  Может вызываться только для объектов классов
  numeric_matrix_object и interval_matrix_object.

  \param lhs Левый операнд.
  \param rhs Правый операнд.
  \return Объект, содержащий частное операндов.
 */
matrix_object& operator /(matrix_object &lhs, matrix_object &rhs)
{
    matrix_object *result = 0;

    if(dynamic_cast<numeric_matrix_object*>(&lhs))
    {
        numeric_matrix_object *left_num =
                dynamic_cast<numeric_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            numeric_matrix_object *num_res =
                    new numeric_matrix_object(left_num->getMatrix());
            num_res->getMatrix() /= right_num->getMatrix();
            result = num_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);
            interval_res->getMatrix().assign(left_num->getMatrix());
            interval_res->getMatrix() /= right_interval->getMatrix();
            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else if(dynamic_cast<interval_matrix_object*>(&lhs))
    {
        interval_matrix_object *left_interval =
                dynamic_cast<interval_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(left_interval->getMatrix());
            interval_res->getMatrix() /= right_num->getMatrix();
            result = interval_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(left_interval->getMatrix());
            interval_res->getMatrix() /= right_interval->getMatrix();
            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else
    {
        throw runtime_error("Unknown type");
    }

    return *result;
}

/*!
  \brief Конвертирует объект класса numeric_matrix_object в объект
  класса interval_matrix_object.

  Создает объект класса interval_matrix_object с интервальной матрицей,
  концы интервалов которой равны соответствующим элементам числовой матрицы
  аргумента.

  \param num_obj Конвертируемый объект.
  \return Результат конвертирования.
 */
interval_matrix_object* convertNumericToInterval(numeric_matrix_object *num_obj)
{
    interval_matrix_object *res =
            new interval_matrix_object(num_obj->getRows(), num_obj->getColumns());
    res->getMatrix().assign(num_obj->getMatrix());
    return res;
}

/*!
  \brief Поэлементное произведение матриц, содержащихся в объектах классов,
  производных от matrix_object.

  Может вызываться только для объектов классов
  numeric_matrix_object и interval_matrix_object.

  \param lhs Левый операнд.
  \param rhs Правый операнд.
  \return Объект, содержащий поэлементное произведение операндов.
 */
matrix_object& elementwiseProduct(matrix_object &lhs, matrix_object &rhs)
{
    matrix_object *result = 0;

    if(dynamic_cast<numeric_matrix_object*>(&lhs))
    {
        numeric_matrix_object *left_num =
                dynamic_cast<numeric_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            numeric_matrix_object *num_res = 0;
            num_res = new numeric_matrix_object(left_num->getMatrix());
            num_res->getMatrix().elementwiseMult(right_num->getMatrix());

            result = num_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);

            interval_res->getMatrix().assign(left_num->getMatrix());

            interval_res->getMatrix().elementwiseMult(right_interval->getMatrix());

            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else if(dynamic_cast<interval_matrix_object*>(&lhs))
    {
        interval_matrix_object *left_interval =
                dynamic_cast<interval_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            interval_matrix_object *interval_res = 0;

            interval_res = new interval_matrix_object(left_interval->getMatrix());

            interval_res->getMatrix().elementwiseMult(right_num->getMatrix());

            result = interval_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);

            interval_res->getMatrix().assign(left_interval->getMatrix());

            interval_res->getMatrix().elementwiseMult(right_interval->getMatrix());

            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else
    {
        throw runtime_error("Unknown type");
    }

    return *result;
}

/*!
  \brief Кронекерово произведение матриц, содержащихся в объектах классов,
  производных от matrix_object.

  Может вызываться только для объектов классов
  numeric_matrix_object и interval_matrix_object.

  \param lhs Левый операнд.
  \param rhs Правый операнд.
  \return Объект, содержащий Кронекерово произведение операндов.
 */
matrix_object& kronekerProduct(matrix_object &lhs, matrix_object &rhs)
{
    matrix_object *result = 0;

    if(dynamic_cast<numeric_matrix_object*>(&lhs))
    {
        numeric_matrix_object *left_num =
                dynamic_cast<numeric_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            numeric_matrix_object *num_res =
                    new numeric_matrix_object(0, 0);
            matrix<double>::kronekerProduct(num_res->getMatrix(),
                                            left_num->getMatrix(), right_num->getMatrix());

            result = num_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            matrix<interval_double> left_interval_matr;
            left_interval_matr.assign(left_num->getMatrix());

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);

            matrix<interval_double>::kronekerProduct(interval_res->getMatrix(),
                                            left_interval_matr, right_interval->getMatrix());

            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else if(dynamic_cast<interval_matrix_object*>(&lhs))
    {
        interval_matrix_object *left_interval =
                dynamic_cast<interval_matrix_object*>(&lhs);
        if(dynamic_cast<numeric_matrix_object*>(&rhs))
        {
            numeric_matrix_object *right_num =
                    dynamic_cast<numeric_matrix_object*>(&rhs);

            matrix<interval_double> right_interval_matr;
            right_interval_matr.assign(right_num->getMatrix());

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);

            matrix<interval_double>::kronekerProduct(interval_res->getMatrix(),
                                            left_interval->getMatrix(), right_interval_matr);

            result = interval_res;
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            interval_matrix_object *interval_res =
                    new interval_matrix_object(0, 0);

            matrix<interval_double>::kronekerProduct(interval_res->getMatrix(),
                                            left_interval->getMatrix(), right_interval->getMatrix());

            result = interval_res;
        }
        else
        {
            throw runtime_error("Unknown type");
        }
    }
    else
    {
        throw runtime_error("Unknown type");
    }

    return *result;
}



//*************************************************************************************

}

#endif
