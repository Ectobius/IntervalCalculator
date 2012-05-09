#ifndef OBJECT_STORAGE_H
#define OBJECT_STORAGE_H

#include <string>
#include <map>
#include <vector>

#include "matrix.h"
#include "expr_except.h"
#include "boost/numeric/interval.hpp"

namespace int_calc
{
class stored_object
{
public:
    virtual ~stored_object() { }

    virtual stored_object* copy() = 0;

};

class matrix_object : public stored_object
{
public:
    virtual ~matrix_object() { }

    virtual size_t getRows() = 0;
    virtual size_t getColumns() = 0;

    virtual stored_object* copy() = 0;

};

class numeric_matrix_object : public matrix_object
{
public:
    numeric_matrix_object(matrix<double> &matr) :
        matrix_variable(matr)
    {
    }

    numeric_matrix_object(size_t r, size_t c) :
        matrix_variable(r, c)
    {
    }

    matrix<double>& getMatrix() { return matrix_variable; }
    void setMarix(matrix<double> &matr) { matrix_variable = matr; }

    virtual size_t getRows() { return matrix_variable.getRows(); }
    virtual size_t getColumns() { return matrix_variable.getColumns(); }

    virtual stored_object* copy() {
        numeric_matrix_object *obj = new numeric_matrix_object(*this);
        return obj;
    }

    virtual ~numeric_matrix_object() { }
private:
    matrix<double> matrix_variable;
};

class interval_matrix_object : public matrix_object
{
public:
    interval_matrix_object(matrix< boost::numeric::interval<double> > &matr) :
        matrix_variable(matr)
    {
    }

    interval_matrix_object(size_t r, size_t c) :
        matrix_variable(r, c)
    {
    }

    matrix< boost::numeric::interval<double> >&
    getMatrix() { return matrix_variable; }

    void setMatrix(matrix< boost::numeric::interval<double> > &matr)
    {
        matrix_variable = matr;
    }

    virtual size_t getRows() { return matrix_variable.getRows(); }
    virtual size_t getColumns() { return matrix_variable.getColumns(); }

    virtual ~interval_matrix_object() { }

    virtual stored_object* copy() {
        interval_matrix_object *obj = new interval_matrix_object(*this);
        return obj;
    }

private:
    matrix< boost::numeric::interval<double> > matrix_variable;
};

class function_object : public stored_object
{
public:
    virtual stored_object* operator ()(std::vector<stored_object*> &args) = 0;

    virtual ~function_object() { }

    virtual stored_object* copy() = 0;
};


//*****************************************************************************
matrix_object& operator +(matrix_object &lhs, matrix_object &rhs);
matrix_object& operator -(matrix_object &lhs, matrix_object &rhs);
matrix_object& operator *(matrix_object &lhs, matrix_object &rhs);
matrix_object& operator /(matrix_object &lhs, matrix_object &rhs);
//*****************************************************************************


class object_storage
{
public:
    virtual void addObject(std::string &, stored_object *) = 0;
    virtual void deleteObject(std::string &) = 0;
    virtual stored_object* getObjectByName(const std::string&) = 0;
    virtual void clear() = 0;

    virtual stored_object* getFirst() = 0;
    virtual stored_object* getNext() = 0;

    virtual ~object_storage() { }

    object_storage();
};

class map_object_storage : public object_storage
{
public:
    virtual void addObject(std::string &, stored_object *);
    virtual void deleteObject(std::string &);
    virtual stored_object* getObjectByName(const std::string&);
    virtual void clear();

    virtual stored_object* getFirst();
    virtual stored_object* getNext();

    virtual ~map_object_storage() { destroy(); }


private:
    typedef std::map<std::string, stored_object*> storage_type;

    std::map<std::string, stored_object*> storage;

    std::map<std::string, stored_object*>::iterator cur_object;

    void destroy();
};

}

#endif // OBJECT_STORAGE_H
