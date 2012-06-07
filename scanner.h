#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "expr_except.h"

namespace int_calc
{

class scanner
{
public:

    enum lexem_type
    {
        identifier,
        number,
        assignment,
        plus,
        minus,
        mult,
        div,
        opening_bracket,
        closing_bracket,
        opening_square_bracket,
        closing_square_bracket,
        semicolon,
        opening_brace,
        closing_brace,
        comma,
        command,
        end_expression,
        error_type
    };

    typedef std::string::size_type position_type;

    scanner();
    scanner(const std::string &str) :
        text(str), pos(0) { }
    const std::string& getText() const { return text; }
    void setText(const std::string &str) { text = str; pos = 0; }

    lexem_type scanNext(std::string &lex);
    position_type getPosition() { return pos; }
    void setPosition(position_type new_pos) { pos = new_pos; }

private:
    std::string text;
    position_type pos;
};

}

#endif // SCANNER_H
