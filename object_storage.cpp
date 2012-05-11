#ifndef OBJECT_STORAGE_CPP
#define OBJECT_STORAGE_CPP

#include "object_storage.h"

#include <algorithm>

using boost::numeric::interval;

namespace int_calc
{

object_storage::object_storage()
{
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

void map_object_storage::deleteObject(std::string &nam)
{
    storage_type::iterator iter = storage.find(nam);
    storage.erase(iter);
}

stored_object* map_object_storage::getObjectByName(const std::string &nm)
{
    storage_type::iterator iter = storage.find(nm);
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

stored_object* map_object_storage::getFirst()
{
    cur_object = storage.begin();
    if(cur_object != storage.end())
    {
        return cur_object->second;
    }
    else
    {
        return 0;
    }
}

stored_object* map_object_storage::getNext()
{
    ++cur_object;
    if(cur_object != storage.end())
    {
        return cur_object->second;
    }
    else
    {
        return 0;
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

//Оператор минус
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

            matrix< d_interval >::multiply(interval_res->getMatrix(),
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

            matrix< d_interval >::multiply(interval_res->getMatrix(),
                                     left_interval->getMatrix(), right_num->getMatrix());
        }
        else if(dynamic_cast<interval_matrix_object*>(&rhs))
        {
            interval_matrix_object *right_interval =
                    dynamic_cast<interval_matrix_object*>(&rhs);

            matrix< d_interval >::multiply(interval_res->getMatrix(),
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



//*************************************************************************************

}

#endif
