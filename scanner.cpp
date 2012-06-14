#include "scanner.h"
#include <cctype>

using namespace std;
using namespace int_calc;

/*!
  \brief Состояния сканера.
 */
enum scanner_state
{
    initial,            /*!< Начальное состояние. */
    scan_identifier,    /*!< Сканирование идентификатора. */
    scan_number1,       /*!< Сканирование числа 1. */
    scan_number2,       /*!< Сканирование числа 2. */
    scan_number3,       /*!< Сканирование числа 3. */
    scan_number4,       /*!< Сканирование числа 4. */
    scan_number5,       /*!< Сканирование числа 5. */
    scan_number6,       /*!< Сканирование числа 6. */
    skipping_spaces,    /*!< Пропуск пробелов. */
    final               /*!< Конечное состояние. */
};

/*!
  \brief Сканирование очередной лексемы.
  \param lex Выходной параметр - изображение лексемы.
  \return Тип отсканированной лексемы.
 */
scanner::lexem_type scanner::scanNext(std::string &lex)
{
    lex = string();
    scanner_state cur_state = initial;
    lexem_type result = scanner::error_type;
    bool isCmd = false;

    while(cur_state != final)
    {
        bool doInc = true;
        switch(cur_state)
        {
        case initial:
            if(pos >= text.size())
            {
                cur_state = final;
                result = end_expression;
                doInc = false;
            }
            else if(isalpha(text[pos]))
            {
                lex.push_back(text[pos]);
                cur_state = scan_identifier;
            }
            else if(isdigit(text[pos]))
            {
                lex.push_back(text[pos]);
                cur_state = scan_number1;
            }
            else if(isspace(text[pos]))
            {
                cur_state = skipping_spaces;
            }
            else if (text[pos] == '#')
            {
                lex.push_back(text[pos]);
                cur_state = scan_identifier;
                isCmd = true;
            }
            else
            {
                bool isKnown = true;
                switch(text[pos])
                {
                case '=':
                    result = assignment;
                    break;
                case '+':
                    result = plus;
                    break;
                case '-':
                    result = minus;
                    break;
                case '*':
                    result = mult;
                    break;
                case '/':
                    result = div;
                    break;
                case '(':
                    result = opening_bracket;
                    break;
                case ')':
                    result = closing_bracket;
                    break;
                case '[':
                    result = opening_square_bracket;
                    break;
                case ']':
                    result = closing_square_bracket;
                    break;
                case ';':
                    result = semicolon;
                    break;
                case '{':
                    result = opening_brace;
                    break;
                case '}':
                    result = closing_brace;
                    break;
                case ',':
                    result = comma;
                    break;
                default:
                    isKnown = false;
                    result = error_type;
                    doInc = false;
                    break;
                }

                if(isKnown)
                {
                    lex.push_back(text[pos]);
                }

                cur_state = final;
            }
            break;  //case: initial
        case scan_identifier:
            if(pos >= text.size() ||
                    (!isalpha(text[pos]) && !isdigit(text[pos])))
            {
                cur_state = final;
                if (isCmd)
                    result = command;
                else
                    result = identifier;
                doInc = false;
            }
            else
            {
                lex.push_back(text[pos]);
            }
            break;  //case: scan_identifier
        case scan_number1:
            if(pos >= text.size())
            {
                cur_state = final;
                result = number;
                doInc = false;
            }
            else if(isdigit(text[pos]))
            {
                lex.push_back(text[pos]);
            }
            else if(text[pos] == '.')
            {
                lex.push_back(text[pos]);
                cur_state = scan_number2;
            }
            else if(text[pos] == 'e' || text[pos] == 'E')
            {
                lex.push_back(text[pos]);
                cur_state = scan_number4;
            }
            else
            {
                cur_state = final;
                result = number;
                doInc = false;
            }
            break;  //case: scan_number1
        case scan_number2:
            if(pos >= text.size() || !isdigit(text[pos]))
            {
                cur_state = final;
                result = error_type;
                doInc = false;
            }
            else
            {
                lex.push_back(text[pos]);
                cur_state = scan_number3;
            }
            break;  //case: scan_number2
        case scan_number3:
            if(pos >= text.size())
            {
                cur_state = final;
                result = number;
                doInc = false;
            }
            else if(isdigit(text[pos]))
            {
                lex.push_back(text[pos]);
            }
            else if(text[pos] == 'e' || text[pos] == 'E')
            {
                lex.push_back(text[pos]);
                cur_state = scan_number4;
            }
            else
            {
                cur_state = final;
                result = number;
                doInc = false;
            }
            break;  //case: scan_number3
        case scan_number4:
            if(pos >= text.size() ||
                    (text[pos] != '+' && text[pos] != '-'))
            {
                cur_state = final;
                result = error_type;
                doInc = false;
            }
            else
            {
                lex.push_back(text[pos]);
                cur_state = scan_number5;
            }
            break;  //case: scan_number4
        case scan_number5:
            if(pos >= text.size() || !isdigit(text[pos]))
            {
                cur_state = final;
                result = error_type;
                doInc = false;
            }
            else
            {
                lex.push_back(text[pos]);
                cur_state = scan_number6;
            }
            break;  //case: scan_number5
        case scan_number6:
            if(pos >= text.size() || !isdigit(text[pos]))
            {
                cur_state = final;
                result = number;
                doInc = false;
            }
            else
            {
                lex.push_back(text[pos]);
            }
            break; // case: scan_number6
        case skipping_spaces:
            if(!isspace(text[pos]))
            {
                cur_state = initial;
                doInc = false;
            }
            break;
        case final:
            break;
        }  // end of switch(cur_state)

        if(doInc)
        {
            ++pos;
        }
    }

    return result;
}
