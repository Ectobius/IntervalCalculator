#include "function_objects.h"

//using namespace std;


namespace int_calc
{

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
        throw runtime_error("Wrong argument type");
    }

    matrix_object *result = &(*left + *right);

    return result;
}


}
