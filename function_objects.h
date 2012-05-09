#ifndef FUNCTION_OBJECTS_H
#define FUNCTION_OBJECTS_H

#include "object_storage.h"
#include "expr_except.h"
#include <vector>

namespace int_calc
{

class func_plus : public function_object
{
    virtual stored_object* operator ()(std::vector<stored_object*> &args);
    virtual ~func_plus() { }

    virtual stored_object* copy() {
        func_plus *obj = new func_plus(*this);
        return obj;
    }
};

}

#endif // FUNCTION_OBJECTS_H
