#include <cmath>
#include "function_objects.h"

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
                dynamic_cast<numeric_matrix_object*>(args[0])->getMatrix().det();
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

}

