#include <cmath>
#include "function_objects.h"
#include "numeric_methods.h"

//using namespace std;


namespace int_calc
{

void load_function_objects(object_storage *stor)
{
    function_object *func = 0;

    func = new func_plus();
    stor->addObject("plus", func);

    func = new func_sin();
    stor->addObject("sin", func);

    func = new func_cos();
    stor->addObject("cos", func);

    func = new func_tan();
    stor->addObject("tan", func);

    func = new func_asin();
    stor->addObject("asin", func);

    func = new func_acos();
    stor->addObject("acos", func);

    func = new func_atan();
    stor->addObject("atan", func);

    func = new func_exp();
    stor->addObject("exp", func);

    func = new func_log();
    stor->addObject("log", func);

    func = new func_abs();
    stor->addObject("abs", func);

    func = new func_sqrt();
    stor->addObject("sqrt", func);

    func = new func_transp();
    stor->addObject("transp", func);

    func = new func_det();
    stor->addObject("det", func);

    func = new func_re_split();
    stor->addObject("resplit", func);

    func = new func_subdiff();
    stor->addObject("subdiff", func);

    func = new func_leverrier();
    stor->addObject("leverrier", func);

    func = new func_is_stable();
    stor->addObject("isStable", func);

}

stored_object* func_plus::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 2)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *left = 0;
    matrix_object *right = 0;

    if(dynamic_cast<matrix_object*>(args[0]))
    {
        left = dynamic_cast<matrix_object*>(args[0]);
    }
    else
    {
        throw runtime_error("Wrong argument type");
    }

    if(dynamic_cast<matrix_object*>(args[1]))
    {
        right = dynamic_cast<matrix_object*>(args[1]);
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }

    matrix_object *result = &(*left + *right);

    return result;
}



stored_object* func_sin::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), sin);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::sin);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_cos::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), cos);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::cos);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_tan::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), tan);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::tan);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_asin::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), asin);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::asin);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_acos::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), acos);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::acos);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_atan::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), atan);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::atan);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_exp::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), exp);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::exp);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_log::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), log);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::log);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_abs::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), abs);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::abs);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_sqrt::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        applyToElements(num_res->getMatrix(), sqrt);
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        applyToConstElements(interval_res->getMatrix(), boost::numeric::sqrt);
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_transp::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(
                    dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        num_res->getMatrix().transp();
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(
                    dynamic_cast<interval_matrix_object*>(args[0])->getMatrix());
        interval_res->getMatrix().transp();
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_det::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }

    matrix_object *res = 0;
    if(dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(1, 1);
        num_res->getMatrix()(0, 0) =
                num_methods::determinant(dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix());
        res = num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(1, 1);
        interval_res->getMatrix()(0, 0) =
                dynamic_cast<interval_matrix_object*>(args[0])->getMatrix().det();
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_re_split::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 5)
    {
        throw runtime_error("Wrong arguments count");
    }

    interval_matrix_object *matr_A = 0,
            *vect_b = 0;
    numeric_matrix_object *eps_obj = 0,
            *tau_obj = 0,
            *iter_count_obj = 0;

    if(!(matr_A = dynamic_cast<interval_matrix_object*>(args[0])) ||
            !(vect_b = dynamic_cast<interval_matrix_object*>(args[1])) ||
            !(eps_obj = dynamic_cast<numeric_matrix_object*>(args[2])) ||
            !(tau_obj = dynamic_cast<numeric_matrix_object*>(args[3])) ||
            !(iter_count_obj = dynamic_cast<numeric_matrix_object*>(args[4])))
    {
        throw wrong_type("Wrong argument type");
    }

    if(matr_A->getRows() != matr_A->getColumns())
    {
        throw wrong_type("Expected square matrix");
    }
    if(vect_b->getColumns() != 1)
    {
        throw wrong_type("Expected column vector");
    }
    if(matr_A->getColumns() != vect_b->getRows())
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    int iter_count = static_cast<int>(iter_count_obj->getMatrix()(0, 0));
    double eps = eps_obj->getMatrix()(0, 0);
    double tau = tau_obj->getMatrix()(0, 0);

    if(iter_count_obj->getMatrix()(0, 0) - iter_count != 0)
    {
        throw wrong_type("Iteration count must be integer");
    }

    int dim = matr_A->getRows();

    double *A = new double[2 * dim * dim];
    double *b = new double[2 * dim];
    double *x = new double[2 * dim];

    for(int i = 0; i < dim; ++i)
    {
        for(int j = 0; j < dim; ++j)
        {
            *(A + 2 * dim * i + 2 * j) = matr_A->getMatrix()(i, j).lower();
            *(A + 2 * dim * i + 2 * j + 1) = matr_A->getMatrix()(i, j).upper();
        }
        *(b + 2 * i) = vect_b->getMatrix()(i, 0).lower();
        *(b + 2 * i + 1) = vect_b->getMatrix()(i, 0).upper();
    }

    int res = num_methods::re_split(matr_A->getRows(), A, b, x, eps, tau, iter_count);

    delete A;
    delete b;

    if(!res)
    {
        delete x;
        throw runtime_error("Matrix of system is i-singular");
    }

    interval_matrix_object *result =
            new interval_matrix_object(dim, 1);

    double left = 0,
            right = 0;
    for(int i = 0; i != dim; ++i)
    {
        left = x[2 * i];
        right = x[2 * i + 1];
        if(left > right)
        {
            delete x;
            throw runtime_error("System has no solutions");
        }
        result->getMatrix()(i, 0) = d_interval(left, right);
    }

    delete x;

    return result;
}

stored_object* func_subdiff::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 5)
    {
        throw runtime_error("Wrong arguments count");
    }

    interval_matrix_object *matr_A = 0,
            *vect_b = 0;
    numeric_matrix_object *eps_obj = 0,
            *tau_obj = 0,
            *iter_count_obj = 0;

    if(!(matr_A = dynamic_cast<interval_matrix_object*>(args[0])) ||
            !(vect_b = dynamic_cast<interval_matrix_object*>(args[1])) ||
            !(eps_obj = dynamic_cast<numeric_matrix_object*>(args[2])) ||
            !(tau_obj = dynamic_cast<numeric_matrix_object*>(args[3])) ||
            !(iter_count_obj = dynamic_cast<numeric_matrix_object*>(args[4])))
    {
        throw wrong_type("Wrong argument type");
    }

    if(matr_A->getRows() != matr_A->getColumns())
    {
        throw wrong_type("Expected square matrix");
    }
    if(vect_b->getColumns() != 1)
    {
        throw wrong_type("Expected column vector");
    }
    if(matr_A->getColumns() != vect_b->getRows())
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    int iter_count = static_cast<int>(iter_count_obj->getMatrix()(0, 0));
    double eps = eps_obj->getMatrix()(0, 0);
    double tau = tau_obj->getMatrix()(0, 0);

    if(iter_count_obj->getMatrix()(0, 0) - iter_count != 0)
    {
        throw wrong_type("Iteration count must be integer");
    }

    int dim = matr_A->getRows();

    double *A = new double[2 * dim * dim];
    double *b = new double[2 * dim];
    double *x = new double[2 * dim];

    for(int i = 0; i < dim; ++i)
    {
        for(int j = 0; j < dim; ++j)
        {
            *(A + 2 * dim * i + 2 * j) = matr_A->getMatrix()(i, j).lower();
            *(A + 2 * dim * i + 2 * j + 1) = matr_A->getMatrix()(i, j).upper();
        }
        *(b + 2 * i) = vect_b->getMatrix()(i, 0).lower();
        *(b + 2 * i + 1) = vect_b->getMatrix()(i, 0).upper();
    }

    int res = num_methods::subdiff(matr_A->getRows(), A, b, x, eps, tau, iter_count);

    delete A;
    delete b;

    if(!res)
    {
        delete x;
        throw runtime_error("Matrix of system is i-singular");
    }

    interval_matrix_object *result =
            new interval_matrix_object(dim, 1);

    double left = 0,
            right = 0;
    for(int i = 0; i != dim; ++i)
    {
        left = x[2 * i];
        right = x[2 * i + 1];
        if(left > right)
        {
            delete x;
            throw runtime_error("System has no solutions");
        }
        result->getMatrix()(i, 0) = d_interval(left, right);
    }

    delete x;

    return result;
}

stored_object* func_leverrier::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }
    if(matrix_object *matr_arg =
            dynamic_cast<matrix_object*>(args[0]))
    {
        if(matr_arg->getRows() != matr_arg->getColumns())
        {
            throw wrong_type("Expected a square matrix");
        }
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }

    matrix_object *res = 0;
    if(numeric_matrix_object *num_arg = dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(1, num_arg->getRows() + 1);
        num_arg->getMatrix().leverrier(num_res->getMatrix());
        res = num_res;
    }
    else if(interval_matrix_object *interval_arg = dynamic_cast<interval_matrix_object*>(args[0]))
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(1, interval_arg->getRows() + 1);
        interval_arg->getMatrix().leverrier(interval_res->getMatrix());
        res = interval_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

stored_object* func_is_stable::operator ()(vector<stored_object*> &args)
{
    if(args.size() != 1)
    {
        throw runtime_error("Wrong arguments count");
    }
    if(matrix_object *matr_arg =
            dynamic_cast<matrix_object*>(args[0]))
    {
        if(matr_arg->getRows() != 1)
        {
            throw wrong_type("Expected row vector");
        }
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }

    matrix_object *res = 0;
    if(numeric_matrix_object *num_arg = dynamic_cast<numeric_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(1, 1);
        num_res->getMatrix()(0, 0) = num_methods::isStablePolynom(num_arg->getMatrix());
        res = num_res;
    }
    else if(interval_matrix_object *interval_arg = dynamic_cast<interval_matrix_object*>(args[0]))
    {
        numeric_matrix_object *num_res =
                new numeric_matrix_object(1, 1);
        num_res->getMatrix()(0, 0) = num_methods::haritonovCritery(interval_arg->getMatrix());
        res = num_res;
    }
    else
    {
        throw wrong_type("Wrong argument type");
    }
    return res;
}

}

