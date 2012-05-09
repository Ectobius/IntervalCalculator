#ifndef EXPR_EXCEPT_H
#define EXPR_EXCEPT_H

#include <stdexcept>

namespace int_calc
{

class parsing_error : public std::runtime_error
{
public:
    parsing_error(const std::string &what_arg) :
        std::runtime_error(what_arg)
    {
    }
};

class wrong_type : public std::runtime_error
{
public:
    wrong_type(const std::string &what_arg) :
        std::runtime_error(what_arg)
    {
    }
};

class undefined_variable : public std::runtime_error
{
public:
    undefined_variable(const std::string &what_arg) :
        std::runtime_error(what_arg)
    {
    }
};

}

#endif // EXPR_EXCEPT_H
