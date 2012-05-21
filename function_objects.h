#ifndef FUNCTION_OBJECTS_H
#define FUNCTION_OBJECTS_H

#include "object_storage.h"
#include "expr_except.h"
#include <vector>

namespace int_calc
{

void load_function_objects(object_storage *stor);


class func_plus : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_sin : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_cos : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_tan : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_asin : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_acos : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_atan : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_exp : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_log : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_abs : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_sqrt : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_width : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_median : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_transp : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_det : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_re_split : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_subdiff : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_leverrier : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_is_stable : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_inv : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_controllability_matrix : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

class func_design_control : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
};

}

#endif // FUNCTION_OBJECTS_H
