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

using boost::numeric::interval;

namespace int_calc
{

class expression_node
{
public:
    virtual stored_object* operator()() = 0;

    virtual ~expression_node() { }
};

class unary_minus_node : public expression_node
{
public:
    unary_minus_node(expression_node *ch) :
        child(ch), result(0)
    {
    }
    virtual stored_object* operator()();
    virtual ~unary_minus_node()
    {
        delete child;
        delete result;
    }

private:
    expression_node *child;
    stored_object *result;
};

class plus_node : public expression_node
{
public:
    plus_node(expression_node *l,
              expression_node *r):
        left(l), right(r), result(0)
    {
    }

    virtual stored_object* operator()();

    virtual ~plus_node()
    {
        delete left;
        delete right;
        delete result;
    }

private:
    expression_node *left;
    expression_node *right;
    stored_object *result;
};

class minus_node : public expression_node
{
public:
    minus_node(expression_node *l,
              expression_node *r):
        left(l), right(r), result(0)
    {
    }

    virtual stored_object* operator()();

    virtual ~minus_node()
    {
        delete left;
        delete right;
        delete result;
    }

private:
    expression_node *left;
    expression_node *right;
    stored_object *result;
};

class multiply_node : public expression_node
{
public:
    multiply_node(expression_node *l,
              expression_node *r):
        left(l), right(r), result(0)
    {
    }

    virtual stored_object* operator()();

    virtual ~multiply_node()
    {
        delete left;
        delete right;
        delete result;
    }

private:
    expression_node *left;
    expression_node *right;
    stored_object *result;
};

class divide_node : public expression_node
{
public:
    divide_node(expression_node *l,
              expression_node *r):
        left(l), right(r), result(0)
    {
    }

    virtual stored_object* operator()();

    virtual ~divide_node()
    {
        delete left;
        delete right;
        delete result;
    }

private:
    expression_node *left;
    expression_node *right;
    stored_object *result;
};

class interval_creation_node : public expression_node
{
public:
    interval_creation_node(expression_node *l,
              expression_node *r):
        left(l), right(r), result(0)
    {
    }

    virtual stored_object* operator ()();

    virtual ~interval_creation_node()
    {
        delete left;
        delete right;
        delete result;
    }

private:
    expression_node *left;
    expression_node *right;
    stored_object *result;
};

class matrix_creation_node : public expression_node
{
public:
    matrix_creation_node() :
        result(0)
    {
    }

    vector< vector<expression_node*> > childs;

    virtual stored_object* operator ()();

    virtual ~matrix_creation_node();

private:
    stored_object *result;
};

class function_call_node : public expression_node
{
public:
    vector<expression_node*> childs;

    function_call_node(function_object *f) :
        func(f), result(0)
    {
    }

    virtual stored_object* operator ()();

    virtual ~function_call_node();

private:
    function_object *func;
    stored_object *result;
};

class variable_node : public expression_node
{
public:
    variable_node(stored_object *var) :
        variable(var)
    {

    }

    virtual stored_object* operator()() { return variable; }
    virtual ~variable_node() { }

private:
    stored_object *variable;
};

class constant_node : public expression_node
{
public:
    constant_node(double val) :
        constant(0)
    {
        numeric_matrix_object *num_obj = new numeric_matrix_object(1, 1);
        num_obj->getMatrix()(0, 0) = val;
        constant = num_obj;
    }

    constant_node(interval<double> ival) :
        constant(0)
    {
        matrix< interval<double> > matr(1, 1);
        constant = new interval_matrix_object(matr);
        (*dynamic_cast<interval_matrix_object*>(constant)).getMatrix()(0, 0) = ival;
    }

    virtual stored_object* operator()() { return dynamic_cast<numeric_matrix_object*>(constant); }
    virtual ~constant_node()
    {
        delete constant;
    }

private:
    stored_object *constant;
};


class expression_interpreter
{
public:
    expression_interpreter(object_storage *);

    string execute(std::string &cmd);


private:
    int_calc::scanner scan;
    object_storage *storage;
    bool result;

    std::string S();
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
