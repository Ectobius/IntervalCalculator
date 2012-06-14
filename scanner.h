#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "expr_except.h"

namespace int_calc
{

/*!
  \brief Класс сканера.
 */
class scanner
{
public:

    /*!
      \brief Типы лексем языка выражений.
     */
    enum lexem_type
    {
        identifier,             /*!< Идентификатор. */
        number,                 /*!< Число. */
        assignment,             /*!< Знак равенства. */
        plus,                   /*!< Знак плюс. */
        minus,                  /*!< Знак минус. */
        mult,                   /*!< Знак умножения. */
        div,                    /*!< Знак деления. */
        opening_bracket,        /*!< Открывающая скобка. */
        closing_bracket,        /*!< Закрывающая скобка. */
        opening_square_bracket, /*!< Открывающая квадратная скобка. */
        closing_square_bracket, /*!< Закрывающая квадратная скобка. */
        semicolon,              /*!< Точка с запятой. */
        opening_brace,          /*!< Открывающая фигурная скобка. */
        closing_brace,          /*!< Закрывающая фигурная скобка. */
        comma,                  /*!< Запятая. */
        command,                /*!< Команда (#identificator). */
        end_expression,         /*!< Конец выражения. */
        error_type              /*!< Ошибочный символ. */
    };

    /*!
      \brief Тип для хранения позиции в тексте.
     */
    typedef std::string::size_type position_type;

    /*!
      \brief Конструктор по умолчанию.
     */
    scanner() :
        pos(0)
    {
    }

    /*!
      \brief Конструктор, инициализирующий строку со сканируемым текстом.
     */
    scanner(const std::string &str) :
        text(str),
        pos(0)
    {
    }

    /*!
      \return Ссылка на сканируемый текст.
     */
    const std::string& getText() const { return text; }
    /*!
      \brief Устанавливает сканируемую строку.
      \param str  Сканируемая строка.
     */
    void setText(const std::string &str) { text = str; pos = 0; }

    lexem_type scanNext(std::string &lex);

    /*!
      \return Текущая позиция в тексте.
     */
    position_type getPosition() { return pos; }

    /*!
      \brief Устанавливает текущую позицию в тексте.
      \param new_pos Устанавливаемая позиция в тексте.
     */
    void setPosition(position_type new_pos) { pos = new_pos; }

private:
    std::string text;   /*!< Сканируемый текст. */
    position_type pos;  /*!< Текущая позиция в тексте. */
};

}

#endif // SCANNER_H
