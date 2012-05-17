#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <vector>
#include "matrix.h"


using namespace std;

namespace int_calc
{

/*Конструктор по умолчанию*/
template <typename T> matrix<T>::matrix(): rows(0), columns(0), matr_ptr(0)
{

}

/*Конструктор, создающий матрицу заданного размера*/
template <typename T> matrix<T>::matrix(size_t r, size_t c): rows(r), columns(c), matr_ptr(0)
{
    matr_ptr = new T*[rows];
    for(size_t i = 0; i != rows; ++i)
    {
        matr_ptr[i] = new T[columns];
    }
}

/*Метод, освобождающий память, занятую матрицей*/
template <typename T> void matrix<T>::destroy()
{
    for(size_t i = 0; i != rows; ++i)
    {
        delete [] matr_ptr[i];
    }
    delete [] matr_ptr;
    matr_ptr = 0;
    rows = 0;
    columns = 0;
}

/*Метод выделяющий память под элементы матрицы и копирующий их из другой матрицы*/
template <typename T> template <typename TR> void
matrix<T>::copy_elems(const matrix<TR>& M)
{
    matr_ptr = new T*[M.rows];
    for(size_t i = 0; i != M.rows; ++i)
    {
        matr_ptr[i] = new T[M.columns];

        for(size_t j = 0; j != M.columns; ++j)
        {
            matr_ptr[i][j] = M.matr_ptr[i][j];
        }
    }

    rows = M.rows;
    columns = M.columns;
}

template <typename T> T** matrix<T>::allocate_elems(size_t r, size_t c)
{
    T** res_ptr = new T*[r];
    for(size_t i = 0; i != r; ++i)
    {
        res_ptr[i] = new T[c];
    }

    return res_ptr;
}

/*Конструктор копирования*/
template <typename T> matrix<T>::matrix(const matrix &M): rows(M.rows), columns(M.columns), matr_ptr(0)
{
    copy_elems(M);
}

/*Оператор присваивания*/
template <typename T> matrix<T>&
matrix<T>::operator=(const matrix<T> &M)
{
    destroy();
    copy_elems(M);
    return *this;
}

/*Деструктор*/
template <typename T> matrix<T>::~matrix()
{
    destroy();
}

/*Метод для заполнения матрицы заданным значением*/
template <typename T> void matrix<T>::fill(const T &orig)
{
    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            matr_ptr[i][j] = orig;
        }
    }
}

/*Метод для присвоения другой матрицы совместимого типа*/
template <typename T> template <typename TR> void matrix<T>::assign(const matrix<TR> &M)
{
    destroy();
    copy_elems(M);
}

/*Метод, копирующий заданную область из другой матрицы*/
template <typename T> template <typename TR> void matrix<T>::copyArea(size_t dest_row, size_t dest_col,
                                     size_t row_count, size_t col_count, matrix<TR> &src,
                                     size_t src_row, size_t src_col)
{
    if(dest_row + row_count > rows ||
            dest_col + col_count > columns ||
            src_row + row_count > src.getRows() ||
            src_col + col_count > src.getColumns())
    {
        throw out_of_range("Specified area is out of range");
    }

    for(size_t i = 0; i != row_count; ++i)
    {
        for(size_t j = 0; j != col_count; ++j)
        {
            matr_ptr[dest_row + i][dest_col + j] =
                    src(src_row + i, src_col + j);
        }
    }
}

/*Метод, возвращающий ссылку на элемент матрицы*/
template <typename T> T& matrix<T>::operator()(size_t r, size_t c)
{
    if(r > rows || c > columns)
    {
        throw out_of_range("Index is out of range");
    }
    return matr_ptr[r][c];
}

/*Константный метод, возвращающий константную ссылку на элемент матрицы*/
template <typename T> const T& matrix<T>::operator()(size_t r, size_t c) const
{
    if(r > rows || c > columns)
    {
        throw out_of_range("Index is out of range");
    }
    return matr_ptr[r][c];
}

/*Составной оператор присвоения для сложения матриц*/
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator+=(const matrix<TR> &rhs)
{
    if(rows != rhs.rows || columns != rhs.columns)
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            matr_ptr[i][j] += rhs.matr_ptr[i][j];
        }
    }

    return *this;
}

/*Составной оператор присвоения для вычитания матриц*/
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator-=(const matrix<TR> &rhs)
{
    if(rows != rhs.rows || columns != rhs.columns)
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            matr_ptr[i][j] -= rhs.matr_ptr[i][j];
        }
    }

    return *this;
}

/*Составной оператор присвоения для деления матрицы на заданное значение*/
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator/=(const matrix<TR> &rhs)
{
    if(rhs.getRows() != 1 || rhs.getColumns() != 1)
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    (*this) /= rhs(0, 0);

    return *this;
}

/*Составной оператор присвоения для сложения матрицы с заданным значением*/
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator+=(const TR &val)
{
    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            matr_ptr[i][j] += val;
        }
    }

    return *this;
}

/*Составной оператор присвоения для вычитания из матрицы заданного значения*/
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator-=(const TR &val)
{
    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            matr_ptr[i][j] -= val;
        }
    }

    return *this;
}

/*Составной оператор присвоения для умножения матрицы на заданное значение*/
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator*=(const TR &val)
{
    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            matr_ptr[i][j] *= val;
        }
    }

    return *this;
}

/*Составной оператор присвоения для деления матрицы на заданное значение*/
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator/=(const TR &val)
{
    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            matr_ptr[i][j] /= val;
        }
    }

    return *this;
}

template <typename T> matrix<T>& matrix<T>::transp()
{
    size_t r = rows;
    size_t c = columns;
    T** new_ptr = allocate_elems(columns, rows);
    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            new_ptr[j][i] = matr_ptr[i][j];
        }
    }
    destroy();
    matr_ptr = new_ptr;
    rows = c;
    columns = r;
    return *this;
}

template <typename T> T matrix<T>::det()
{
    if(rows != columns)
    {
        throw size_mismatch("It is not a square matrix");
    }
    if(rows == 1)
    {
        return matr_ptr[0][0];
    }

    rows_flags = new bool[rows];
    columns_flags = new bool[columns];
    for(size_t i = 0; i < rows; ++i)
        rows_flags[i] = false;
    for(size_t i = 0; i < columns; ++i)
        columns_flags[i] = false;

    T result = recurse_det(rows);
    delete rows_flags;
    delete columns_flags;
    rows_flags = 0;
    columns_flags = 0;
    return result;
}

template <typename T> T matrix<T>::recurse_det(size_t order)
{
    T result = T(0);
    T curDet;
    size_t initRow = 0,
            initCol = 0;
    while(rows_flags[initRow++]);
    while(columns_flags[initCol++]);
    --initRow;
    --initCol;

    if(order == 2)
    {
        size_t secRow = initRow + 1;
        size_t secCol = initCol + 1;
        while(rows_flags[secRow++]);
        while(columns_flags[secCol++]);
        --secRow;
        --secCol;
        result = matr_ptr[initRow][initCol] * matr_ptr[secRow][secCol] -
                matr_ptr[secRow][initCol] * matr_ptr[initRow][secCol];
    }
    else
    {
        rows_flags[initRow] = true;
        for(size_t j = initCol; j != columns; ++j)
        {
            if(!columns_flags[j])
            {
                columns_flags[j] = true;
                curDet = recurse_det(order - 1);
                columns_flags[j] = false;
                if((initRow + j) % 2)
                {
                    result -= matr_ptr[initRow][j] * curDet;
                }
                else
                {
                    result += matr_ptr[initRow][j] * curDet;
                }
            }
        }
        rows_flags[initRow] = false;
    }
    return result;
}

template <typename T> T matrix<T>::trace()
{
    if(rows != columns)
    {
        throw size_mismatch("It is not a square matrix");
    }
    T result = T(0);
    for(size_t i = 0; i != rows; ++i)
    {
        result += matr_ptr[i][i];
    }
    return result;
}

template <typename T> void matrix<T>::leverrier(matrix<T> &pol)
{
    if(rows != columns)
    {
        throw size_mismatch("It is not a square matrix");
    }
    pol.assign(matrix<T>(1, rows + 1));
    pol.fill(T(0));
    pol(0, 0) = 1;

    matrix<T> A(*this);
    matrix<T> mult(*this);
    vector<T> traces(rows);
    T sum(0);
    for(size_t i = 1; i <= rows; ++i)
    {
        traces[i - 1] = A.trace();
        sum = T(0);
        for(size_t j = 0; j < i; ++j)
        {
            sum += pol(0, j) * traces[i - j - 1];
        }
        pol(0, i) = -sum / (double)(i);
        matrix<T>::multiply(mult, A, *this);
        A = mult;
    }

}

/*Метод, осуществляющий умножение матриц*/
template <typename T> template <typename T1, typename T2>
void matrix<T>::multiply(matrix<T> &res, matrix<T1> &matr1, matrix<T2> &matr2)
{
    if(matr1.getRows() == 1 &&
            matr1.getColumns() == 1)
    {
        res.assign(matr2);
        res *= matr1(0, 0);
    }
    else if(matr2.getRows() == 1 &&
            matr2.getColumns() == 1)
    {
        res.assign(matr1);
        res *= matr2(0, 0);
    }
    else if(matr1.getColumns() != matr2.getRows())
    {
        throw size_mismatch("Sizes of matrices do not match");
    }
    else
    {
        if(res.getRows() != matr1.getRows() || res.getColumns() != matr2.getColumns())
        {
            matrix<T> *matr = new matrix<T>(matr1.getRows(), matr2.getColumns());
            res.assign(*matr);
            delete matr;
        }

        T sum(0);
        for(size_t i = 0; i != matr1.getRows(); ++i)
        {
            for(size_t j = 0; j != matr2.getColumns(); ++j)
            {
                sum = 0;
                for(size_t k = 0; k != matr1.getColumns(); ++k)
                {
                    sum += matr1.matr_ptr[i][k] * matr2.matr_ptr[k][j];
                }
                res.matr_ptr[i][j] = sum;
            }
        }
    }
}

template <typename T> std::ostream& operator <<(ostream &os, const matrix<T> &M)
{
    for(size_t i = 0; i != M.getRows(); ++i)
    {
        for(size_t j = 0; j != M.getColumns(); ++j)
        {
            os << M(i, j) << " ";
        }
        os << std::endl;
    }
    return os;
}

template <typename T> bool* matrix<T>::rows_flags = 0;
template <typename T> bool* matrix<T>::columns_flags = 0;

}

#endif // MATRIX_CPP
