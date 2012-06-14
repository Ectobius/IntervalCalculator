/*!
  \file expr_except.h

  Определяет классы исключений, используемые в
  приложении IntervalCalculator.
 */

#ifndef EXPR_EXCEPT_H
#define EXPR_EXCEPT_H

#include <stdexcept>

/*!
  Пространство имен, содержащее классы ядра приложения.
 */
namespace int_calc
{

/*!
  \brief Класс исключения, сигнализирующий о синтаксической ошибке
  при интерпретации выражения.
 */
class parsing_error : public std::runtime_error
{
public:
    parsing_error(const std::string &what_arg) :
        std::runtime_error(what_arg)
    {
    }
};

/*!
  \brief Класс исключения, сигнализирующий неподходящем типе
  аргумента при интерпретации выражения.
 */
class wrong_type : public std::runtime_error
{
public:
    wrong_type(const std::string &what_arg) :
        std::runtime_error(what_arg)
    {
    }
};

/*!
  \brief Класс исключения, сигнализирующий о том, что переменая,
  присутствующая в выражении не определена.
 */
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
