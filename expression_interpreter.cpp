#include "expression_interpreter.h"
#include "matrix.h"
#include <cstdlib>

using namespace std;
using boost::numeric::interval;

namespace int_calc
{

/*Вычисление унарного минуса*/
stored_object* unary_minus_node::operator()()
{
    stored_object *child_res = (*child)();
    result = 0;

    if(dynamic_cast<numeric_matrix_object*>(child_res))
    {
        numeric_matrix_object *num_res =
                dynamic_cast<numeric_matrix_object*>(child_res);
        num_res->getMatrix() *= -1;

        return num_res;
    }
    else if(dynamic_cast<interval_matrix_object*>(child_res))
    {
        interval_matrix_object *interv_res =
                dynamic_cast<interval_matrix_object*>(child_res);
        interv_res->getMatrix() *= -1;

        return interv_res;
    }
    else
    {
        throw wrong_type("Unknown type");
    }
}

/*Вычисление бинарного плюса*/
stored_object* plus_node::operator()()
{
    stored_object *left_res = (*left)();
    stored_object *right_res = (*right)();
    result = 0;

    matrix_object *left_matrix = 0;
    matrix_object *right_matrix = 0;

    if(dynamic_cast<matrix_object*>(left_res))
    {
        left_matrix = dynamic_cast<matrix_object*>(left_res);
    }
    else
    {
        throw wrong_type("Unknown type");
    }
    if(dynamic_cast<matrix_object*>(right_res))
    {
        right_matrix = dynamic_cast<matrix_object*>(right_res);
    }
    else
    {
        throw wrong_type("Unknown type");
    }

    result = &(*left_matrix + *right_matrix);

    return result;
}

/*Вычисление бинарного минуса*/
stored_object* minus_node::operator()()
{
    stored_object *left_res = (*left)();
    stored_object *right_res = (*right)();
    result = 0;

    matrix_object *left_matrix = 0;
    matrix_object *right_matrix = 0;

    if(dynamic_cast<matrix_object*>(left_res))
    {
        left_matrix = dynamic_cast<matrix_object*>(left_res);
    }
    else
    {
        throw wrong_type("Unknown type");
    }
    if(dynamic_cast<matrix_object*>(right_res))
    {
        right_matrix = dynamic_cast<matrix_object*>(right_res);
    }
    else
    {
        throw wrong_type("Unknown type");
    }

    result = &(*left_matrix - *right_matrix);

    return result;
}

//Выполнение умножения
stored_object* multiply_node::operator()()
{
    stored_object *left_res = (*left)();
    stored_object *right_res = (*right)();
    result = 0;

    matrix_object *left_matrix = 0;
    matrix_object *right_matrix = 0;

    if(dynamic_cast<matrix_object*>(left_res))
    {
        left_matrix = dynamic_cast<matrix_object*>(left_res);
    }
    else
    {
        throw wrong_type("Unknown type");
    }
    if(dynamic_cast<matrix_object*>(right_res))
    {
        right_matrix = dynamic_cast<matrix_object*>(right_res);
    }
    else
    {
        throw wrong_type("Unknown type");
    }

    result = &(*left_matrix * *right_matrix);

    return result;
}

stored_object* divide_node::operator()()
{
    stored_object *left_res = (*left)();
    stored_object *right_res = (*right)();
    result = 0;

    matrix_object *left_matrix = 0;
    matrix_object *right_matrix = 0;

    if(dynamic_cast<matrix_object*>(left_res))
    {
        left_matrix = dynamic_cast<matrix_object*>(left_res);
    }
    else
    {
        throw wrong_type("Unknown type");
    }
    if(dynamic_cast<matrix_object*>(right_res))
    {
        right_matrix = dynamic_cast<matrix_object*>(right_res);
    }
    else
    {
        throw wrong_type("Unknown type");
    }

    result = &(*left_matrix / *right_matrix);

    return result;
}

stored_object* interval_creation_node::operator()()
{
    stored_object *left_res = (*left)();
    stored_object *right_res = (*right)();


    if(!dynamic_cast<numeric_matrix_object*>(left_res) ||
            !dynamic_cast<numeric_matrix_object*>(left_res))
    {
        throw wrong_type("Type is not match");
    }

    numeric_matrix_object *left_num_res =
            dynamic_cast<numeric_matrix_object*>(left_res);
    numeric_matrix_object *right_num_res =
            dynamic_cast<numeric_matrix_object*>(right_res);

    if(left_num_res->getMatrix().getRows() != 1 ||
            left_num_res->getMatrix().getColumns() != 1 ||
            right_num_res->getMatrix().getRows() != 1 ||
            right_num_res->getMatrix().getColumns() != 1)
    {
        throw wrong_type("Type is not match");
    }

    interval<double> interv(left_num_res->getMatrix()(0, 0),
                            right_num_res->getMatrix()(0, 0));

    interval_matrix_object *interval_res =
            new interval_matrix_object(1, 1);
    interval_res->getMatrix()(0, 0) = interv;

    result = interval_res;

    return interval_res;
}

stored_object* matrix_creation_node::operator ()()
{
    vector< vector<stored_object*> > childs_res;
    size_t rows_count = 0,
            columns_count = 0,
            cur_columns_count = 0,
            cur_rows_count = 0;
    bool isInterval = false;
    result = 0;

    for(vector< vector<expression_node*> >::size_type i = 0;
        i != childs.size(); ++i)
    {
        childs_res.push_back(vector<stored_object*>());
        cur_columns_count = 0;
        for(vector<expression_node*>::size_type j = 0; j != childs[i].size(); ++j)
        {
            stored_object* res = (*childs[i][j])();
            childs_res[i].push_back(res);

            if(dynamic_cast<matrix_object*>(res))
            {
                matrix_object *matr_res =
                        dynamic_cast<matrix_object*>(res);

                if(j == 0)
                {
                    cur_rows_count = matr_res->getRows();
                    rows_count += cur_rows_count;
                }
                else
                {
                    if(matr_res->getRows() !=
                            cur_rows_count)
                    {
                        //Error
                        throw size_mismatch("Sizes do not match");
                    }
                }

                if(dynamic_cast<interval_matrix_object*>(res))
                {
                    isInterval = true;
                }

                cur_columns_count += matr_res->getColumns();
            }
            else
            {
                throw wrong_type("Unknown type");
            }
        }

        if(i == 0)
        {
            columns_count += cur_columns_count;
        }
        else
        {
            if(cur_columns_count != columns_count)
            {
                //Error
                throw size_mismatch("Size do not match");
            }
        }
    }

    if(isInterval)
    {
        interval_matrix_object *interval_res =
                new interval_matrix_object(rows_count, columns_count);

        size_t dest_row = 0;
        for(vector< vector<stored_object*> >::size_type i = 0;
            i != childs_res.size(); ++i)
        {
            size_t dest_col = 0;
            for(vector<stored_object*>::size_type j = 0; j != childs_res[i].size(); ++j)
            {
                if(dynamic_cast<interval_matrix_object*>(childs_res[i][j]))
                {
                    interval_matrix_object *interv_matr =
                            dynamic_cast<interval_matrix_object*>(childs_res[i][j]);
                    interval_res->getMatrix().copyArea(dest_row, dest_col,
                                                       interv_matr->getMatrix().getRows(),
                                                       interv_matr->getMatrix().getColumns(),
                                                       interv_matr->getMatrix(), 0, 0);
                    dest_col += interv_matr->getMatrix().getColumns();
                    if(j == childs_res[i].size() - 1)
                    {
                        dest_row += interv_matr->getMatrix().getRows();
                    }
                }
                else if(dynamic_cast<numeric_matrix_object*>(childs_res[i][j]))
                {
                    numeric_matrix_object *num_matr =
                            dynamic_cast<numeric_matrix_object*>(childs_res[i][j]);
                    interval_res->getMatrix().copyArea(dest_row, dest_col,
                                                       num_matr->getMatrix().getRows(),
                                                       num_matr->getMatrix().getColumns(),
                                                       num_matr->getMatrix(), 0, 0);
                    dest_col += num_matr->getMatrix().getColumns();
                    if(j == childs_res[i].size() - 1)
                    {
                        dest_row += num_matr->getMatrix().getRows();
                    }
                }

            }
        }

        result = interval_res;
    }
    else
    {
        numeric_matrix_object *numeric_res =
                new numeric_matrix_object(rows_count, columns_count);

        size_t dest_row = 0;
        for(vector< vector<stored_object*> >::size_type i = 0;
            i != childs_res.size(); ++i)
        {
            size_t dest_col = 0;
            for(vector<stored_object*>::size_type j = 0; j != childs_res[i].size(); ++j)
            {
                numeric_matrix_object *num_matr =
                        dynamic_cast<numeric_matrix_object*>(childs_res[i][j]);
                numeric_res->getMatrix().copyArea(dest_row, dest_col,
                                                   num_matr->getMatrix().getRows(),
                                                   num_matr->getMatrix().getColumns(),
                                                   num_matr->getMatrix(), 0, 0);
                dest_col += num_matr->getMatrix().getColumns();
                if(j == childs_res[i].size() - 1)
                {
                    dest_row += num_matr->getMatrix().getRows();
                }
            }
        }

        result = numeric_res;
    }

    return result;
}

matrix_creation_node::~matrix_creation_node()
{
    for(vector< vector<expression_node*> >::size_type i = 0;
        i != childs.size(); ++i)
    {
        for(vector<expression_node*>::size_type j = 0; j != childs[i].size(); ++j)
        {
            delete childs[i][j];
        }
    }

    delete result;
}

stored_object* function_call_node::operator ()()
{
    vector<stored_object*> childs_res;
    stored_object *cur_result = 0;
    result = 0;

    for(vector<expression_node*>::size_type i = 0; i != childs.size(); ++i)
    {
        cur_result = (*childs[i])();
        childs_res.push_back(cur_result);
    }

    result = (*func)(childs_res);

    return result;
}

function_call_node::~function_call_node()
{
    for(vector<expression_node*>::size_type i = 0; i != childs.size(); ++i)
    {
        delete childs[i];
    }

    delete result;
}


expression_interpreter::expression_interpreter(object_storage *stor) :
    storage(stor)
{
}


stored_object* appeal_elements::operator ()(vector<stored_object*> &args)
{
    matrix_object *res = 0;

    if (args.size() == 3)
    {
        int row = 0, col = 0;

        if (numeric_matrix_object *row_ind =
                dynamic_cast<numeric_matrix_object*>(args[1]))
        {
            row = (int)row_ind->getMatrix()(0, 0);
            if ((double)row != row_ind->getMatrix()(0, 0))
            {
                throw runtime_error("Expected integer number");
            }
        }
        else
        {
            throw runtime_error("Wrong argument type");
        }

        if (numeric_matrix_object *col_ind =
                dynamic_cast<numeric_matrix_object*>(args[2]))
        {
            col = (int)col_ind->getMatrix()(0, 0);
            if ((double)col != col_ind->getMatrix()(0, 0))
            {
                throw runtime_error("Expected integer number");
            }
        }
        else
        {
            throw runtime_error("Wrong argument type");
        }

        if (numeric_matrix_object *num_arg =
                dynamic_cast<numeric_matrix_object*>(args[0]))
        {
            numeric_matrix_object *num_res =
                    new numeric_matrix_object(1, 1);
            num_res->getMatrix()(0, 0) = num_arg->getMatrix()(row - 1, col - 1);

            res = num_res;
        }
        else if (interval_matrix_object *interval_arg =
                 dynamic_cast<interval_matrix_object*>(args[0]))
        {
            interval_matrix_object *interval_res =
                    new interval_matrix_object(1, 1);
            interval_res->getMatrix()(0, 0) = interval_arg->getMatrix()(row - 1, col - 1);

            res = interval_res;
        }
    }
    else if (args.size() == 5)
    {
        int row_beg = 0, col_beg = 0,
                row_end = 0, col_end = 0;

        if (numeric_matrix_object *row_ind =
                dynamic_cast<numeric_matrix_object*>(args[1]))
        {
            row_beg = (int)row_ind->getMatrix()(0, 0);
            if ((double)row_beg != row_ind->getMatrix()(0, 0))
            {
                throw runtime_error("Expected integer number");
            }
        }
        else
        {
            throw runtime_error("Wrong argument type");
        }

        if (numeric_matrix_object *row_ind =
                dynamic_cast<numeric_matrix_object*>(args[2]))
        {
            row_end = (int)row_ind->getMatrix()(0, 0);
            if ((double)row_end != row_ind->getMatrix()(0, 0))
            {
                throw runtime_error("Expected integer number");
            }
        }
        else
        {
            throw runtime_error("Wrong argument type");
        }

        if (numeric_matrix_object *col_ind =
                dynamic_cast<numeric_matrix_object*>(args[3]))
        {
            col_beg = (int)col_ind->getMatrix()(0, 0);
            if ((double)col_beg != col_ind->getMatrix()(0, 0))
            {
                throw runtime_error("Expected integer number");
            }
        }
        else
        {
            throw runtime_error("Wrong argument type");
        }

        if (numeric_matrix_object *col_ind =
                dynamic_cast<numeric_matrix_object*>(args[4]))
        {
            col_end = (int)col_ind->getMatrix()(0, 0);
            if ((double)col_end != col_ind->getMatrix()(0, 0))
            {
                throw runtime_error("Expected integer number");
            }
        }
        else
        {
            throw runtime_error("Wrong argument type");
        }

        int rows = row_end - row_beg + 1,
                columns = col_end - col_beg + 1;

        if (rows <= 0 || columns <= 0)
        {
            throw runtime_error("Incorrect range");
        }

        if (numeric_matrix_object *num_arg =
                dynamic_cast<numeric_matrix_object*>(args[0]))
        {
            numeric_matrix_object *num_res =
                    new numeric_matrix_object(rows, columns);

            num_res->getMatrix().copyArea(0, 0, rows, columns,
                                          num_arg->getMatrix(), row_beg - 1, col_beg - 1);

            res = num_res;
        }
        else if (interval_matrix_object *interval_arg =
                 dynamic_cast<interval_matrix_object*>(args[0]))
        {
            interval_matrix_object *interval_res =
                    new interval_matrix_object(rows, columns);

            interval_res->getMatrix().copyArea(0, 0, rows, columns,
                                          interval_arg->getMatrix(), row_beg - 1, col_beg - 1);

            res = interval_res;
        }
    }
    else
    {
        throw runtime_error("Wrong arguments count");
    }

    return res;
}



/********__________________Анализ_____________________**********************
****************************************************************************/

expression_result* expression_interpreter::execute(std::string &cmd)
{
    scan.setText(cmd);    

    result = S();

    return result;
}

expression_result* expression_interpreter::S()
{
    expression_result *res = 0;
    string var_name("ans");
    string lex;
    scanner::position_type pos = scan.getPosition();
    scanner::lexem_type type1 = scan.scanNext(lex);
    if (type1 == scanner::identifier)
    {
        string nm = lex;

        type1 = scan.scanNext(lex);
        if (type1 != scanner::assignment)
        {
            scan.setPosition(pos);
        }
        else
        {
            var_name = nm;
        }

        res = new assign_result(var_name);
    }
    else if (type1 == scanner::command)
    {
        scan.setPosition(pos);
        res = C();
        return res;
    }
    else
    {
        scan.setPosition(pos);
    }

    expression_node *root = E();

    type1 = scan.scanNext(lex);
    if (type1 != scanner::end_expression)
    {
        delete root;
        throw parsing_error("Excess symbols");
    }
    else
    {
        stored_object *res = (*root)()->copy();
        delete root;

        stored_object *assigned_obj = storage->getObjectByName(var_name);
        if(dynamic_cast<function_object*>(assigned_obj))
        {
            throw wrong_type("Left side of assignment cannot be a function");
        }
        else if(assigned_obj)
        {
            storage->deleteObject(var_name);
            delete assigned_obj;
        }

        storage->addObject(var_name, res);
    }

    return res;
}

expression_result* expression_interpreter::C()
{
    expression_result *res = 0;
    string lex;
    scanner::lexem_type type1 = scan.scanNext(lex);

    if (type1 == scanner::command)
    {
        res = new command_result(lex);
    }

    return res;
}

expression_node* expression_interpreter::E()
{
    expression_node *result_node = 0;
    string lex;
    scanner::position_type pos = scan.getPosition();
    scanner::lexem_type type1 = scan.scanNext(lex);

    bool doMinus = false;

    if (type1 != scanner::plus &&
            type1 != scanner::minus)
    {
        scan.setPosition(pos);       
    }

    if (type1 == scanner::minus)
    {
        doMinus = true;
    }

    expression_node *left_node = A(0, scanner::error_type);

    if (doMinus)
    {
        left_node = new unary_minus_node(left_node);
    }

    pos = scan.getPosition();
    type1 = scan.scanNext(lex);
    if (type1 == scanner::plus ||
            type1 == scanner::minus)
    {
        result_node = U(left_node, type1);
    }
    else
    {
        scan.setPosition(pos);
        result_node = left_node;
    }

    return result_node;
}

expression_node* expression_interpreter::U(expression_node *left_node, scanner::lexem_type op)
{
    expression_node *result_node = A(0, scanner::error_type);

    if(left_node)
    {
        if(op == scanner::plus)
        {
            result_node = new plus_node(left_node, result_node);
        }
        else
        {
            result_node = new minus_node(left_node, result_node);
        }
    }

    string lex;
    scanner::position_type pos = scan.getPosition();
    scanner::lexem_type type1 = scan.scanNext(lex);
    if(type1 == scanner::plus ||
            type1 == scanner::minus)
    {
        result_node = U(result_node, type1);
    }
    else
    {
        scan.setPosition(pos);
    }

    return result_node;
}

expression_node* expression_interpreter::A(expression_node *left_node, scanner::lexem_type op)
{
    expression_node *result_node = M();

    if(left_node)
    {
        if(op == scanner::mult)
        {
            result_node = new multiply_node(left_node, result_node);
        }
        else
        {
            result_node = new divide_node(left_node, result_node);
        }
    }

    string lex;
    scanner::position_type pos = scan.getPosition();
    scanner::lexem_type type1 = scan.scanNext(lex);
    if(type1 == scanner::mult ||
            type1 == scanner::div)
    {
        result_node = A(result_node, type1);
    }
    else
    {
        scan.setPosition(pos);
    }

    return result_node;
}

expression_node* expression_interpreter::M()
{
    expression_node *result_node = 0;

    string lex;
    scanner::position_type pos = scan.getPosition();
    scanner::lexem_type type1 = scan.scanNext(lex);

    switch(type1)
    {
    case scanner::identifier:
    {
        string var_name = lex;

        pos = scan.getPosition();
        type1 = scan.scanNext(lex);
        if(type1 == scanner::opening_bracket)
        {
            stored_object *obj = storage->getObjectByName(var_name);
            function_object *fun = 0;

            vector<expression_node*> vect;

            if (dynamic_cast<function_object*>(obj))
            {
                fun = dynamic_cast<function_object*>(obj);
            }
            else if(dynamic_cast<matrix_object*>(obj))
            {
                fun = &app_elem;
                variable_node *var_node =
                        new variable_node(obj);
                vect.push_back(var_node);
            }
            else
            {
                throw runtime_error("Unknown type");
            }

            function_call_node *fun_node =
                    new function_call_node(fun);

            P(vect);

            fun_node->childs.insert(fun_node->childs.end(),
                                    vect.begin(), vect.end());

            type1 = scan.scanNext(lex);
            if(type1 != scanner::closing_bracket)
            {
                throw parsing_error("Error");
            }

            //
            result_node = fun_node;
        }
        else
        {
            scan.setPosition(pos);

            stored_object *var = storage->getObjectByName(var_name);
            if(var)
            {
                result_node = new variable_node(var);
            }
            else
            {
                throw undefined_variable("Variable is undefined");
            }
        }
        break;
    }

    case scanner::number:
        result_node = new constant_node(atof(lex.c_str()));

        break;

    case scanner::opening_bracket:
        result_node = E();
        type1 = scan.scanNext(lex);
        if(type1 != scanner::closing_bracket)
        {
            throw parsing_error("Expected )'");
        }
        break;

    case scanner::opening_square_bracket:
    {
        expression_node *left_node = E();
        type1 = scan.scanNext(lex);
        if(type1 != scanner::semicolon)
        {
            throw parsing_error("Expected ;");
        }
        expression_node *right_node = E();
        type1 = scan.scanNext(lex);
        if(type1 != scanner::closing_square_bracket)
        {
            throw parsing_error("Expected ]");
        }

        result_node = new interval_creation_node(left_node, right_node);
        break;
    }

    case scanner::opening_brace:
    {
        pos = scan.getPosition();
        type1 = scan.scanNext(lex);

        matrix_creation_node *matr_node = new
                matrix_creation_node();

        if(type1 != scanner::closing_brace)
        {
            scan.setPosition(pos);
            B(matr_node->childs);

            type1 = scan.scanNext(lex);
            if(type1 != scanner::closing_brace)
            {
                throw parsing_error("Expected }");
            }
        }

        //
        result_node = matr_node;
        break;
    }

    default:
        throw parsing_error("Error");

    }

    return result_node;
}

void expression_interpreter::B(vector< vector<expression_node*> > &vect)
{
    vector<expression_node*> row_vect;

    G(row_vect);
    vect.push_back(row_vect);

    string lex;
    scanner::position_type pos = scan.getPosition();
    scanner::lexem_type type1 = scan.scanNext(lex);
    if(type1 == scanner::semicolon)
    {
        B(vect);
    }
    else
    {
        scan.setPosition(pos);
    }
}

void expression_interpreter::G(vector<expression_node*> &vect)
{
    expression_node *child = E();
    vect.push_back(child);

    string lex;
    scanner::position_type pos = scan.getPosition();
    scanner::lexem_type type1 = scan.scanNext(lex);
    if(/*type1 != scanner::semicolon &&
            type1 != scanner::closing_brace*/
            type1 == scanner::comma)
    {
        //scan.setPosition(pos);
        G(vect);
    }
    else
    {
        scan.setPosition(pos);
    }
}

void expression_interpreter::P(vector<expression_node*> &vect)
{
    expression_node *child = E();
    vect.push_back(child);

    string lex;
    scanner::position_type pos = scan.getPosition();
    scanner::lexem_type type1 = scan.scanNext(lex);
    if(type1 == scanner::comma)
    {
        P(vect);
    }
    else
    {
        scan.setPosition(pos);
    }
}

}
