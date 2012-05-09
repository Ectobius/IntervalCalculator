#ifndef MATRIX_CPP
#define MATRIX_CPP

#include "matrix.h"
#include <iostream>

using namespace std;

namespace int_calc
{

/* онструктор по умолчанию*/
template <typename T> matrix<T>::matrix(): rows(0), columns(0), matr_ptr(0)
{

}

/* онструктор, создающий матрицу заданного размера*/
template <typename T> matrix<T>::matrix(size_t r, size_t c): rows(r), columns(c), matr_ptr(0)
{
    matr_ptr = new T*[rows];
    for(size_t i = 0; i != rows; ++i)
    {
        matr_ptr[i] = new T[columns];
    }
}

/*ћетод, освобождающий пам€ть, зан€тую матрицей*/
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

/*ћетод выдел€ющий пам€ть под элементы матрицы и копирующий их из другой матрицы*/
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

/* онструктор копировани€*/
template <typename T> matrix<T>::matrix(const matrix &M): rows(M.rows), columns(M.columns), matr_ptr(0)
{
    copy_elems(M);
}

/*ќператор присваивани€*/
template <typename T> matrix<T>&
matrix<T>::operator=(const matrix<T> &M)
{
    destroy();
    copy_elems(M);
    return *this;
}

/*ƒеструктор*/
template <typename T> matrix<T>::~matrix()
{
    destroy();
}

/*ћетод дл€ заполнени€ матрицы заданным значением*/
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

/*ћетод дл€ присвоени€ другой матрицы совместимого типа*/
template <typename T> template <typename TR> void matrix<T>::assign(const matrix<TR> &M)
{
    destroy();
    copy_elems(M);
}

/*ћетод, копирующий заданную область из другой матрицы*/
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

/*ћетод, возвращающий ссылку на элемент матрицы*/
template <typename T> T& matrix<T>::operator()(size_t r, size_t c)
{
    if(r > rows || c > columns)
    {
        throw out_of_range("Index is out of range");
    }
    return matr_ptr[r][c];
}

/* онстантный метод, возвращающий константную ссылку на элемент матрицы*/
template <typename T> const T& matrix<T>::operator()(size_t r, size_t c) const
{
    if(r > rows || c > columns)
    {
        throw out_of_range("Index is out of range");
    }
    return matr_ptr[r][c];
}

/*—оставной оператор присвоени€ дл€ сложени€ матриц*/
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

/*—оставной оператор присвоени€ дл€ вычитани€ матриц*/
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

/*—оставной оператор присвоени€ дл€ делени€ матрицы на заданное значение*/
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator/=(const matrix<TR> &rhs)
{
    if(rhs.getRows() != 1 || rhs.getColumns() != 1)
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    (*this) /= rhs(0, 0);

    return *this;
}

/*—оставной оператор присвоени€ дл€ сложени€ матрицы с заданным значением*/
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

/*—оставной оператор присвоени€ дл€ вычитани€ из матрицы заданного значени€*/
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

/*—оставной оператор присвоени€ дл€ умножени€ матрицы на заданное значение*/
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

/*—оставной оператор присвоени€ дл€ делени€ матрицы на заданное значение*/
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

/*ћетод, осуществл€ющий умножение матриц*/
template <typename T> template <typename T1, typename T2>
void matrix<T>::multiply(matrix<T> &res, matrix<T1> &matr1, matrix<T2> &matr2)
{
    if(matr1.getRows() == 1 &&
            matr1.getRows() == 1)
    {
        res.assign(matr2);
        res *= matr1(0, 0);
    }
    else if(matr2.getRows() == 1 &&
            matr2.getRows() == 1)
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

template <typename T> void print_matr(ostream &os, const matrix<T> &M)
{
    for(size_t i = 0; i != M.getRows(); ++i)
    {
        for(size_t j = 0; j != M.getColumns(); ++j)
        {
            os << M(i, j) << " ";
        }
        os << std::endl;
    }
}

}

#endif // MATRIX_CPP
