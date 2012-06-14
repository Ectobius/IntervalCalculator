#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>
#include <stdexcept>
#include <iostream>

/*!
  Пространство имен, содержащее классы ядра приложения.
 */
namespace int_calc
{
/*!
  Класс исключения, сигнализирующего о несоответствии размеров
  матриц при выполнении операций над матрицами и вычислении
  функций от матриц
 */
class size_mismatch : public std::runtime_error
{
public:
    size_mismatch(const std::string &what_arg) :
        std::runtime_error(what_arg)
    {
    }
};

/*!
  \brief Шаблонный класс, представляющий матрицу.
  \tparam T Тип элементов матрицы.
 */
template <typename T> class matrix
{
    template <typename T2> friend class matrix;
public:
    matrix();
    matrix(size_t r, size_t c);
    matrix(const matrix&);

    matrix& operator=(const matrix&);

    ~matrix();

    /*!
      \return Количество строк матрицы.
     */
    const size_t getRows() const { return rows; }

    /*!
      \return Количество столбцов матрицы.
     */
    const size_t getColumns() const { return columns; }

    void resize(size_t newRows, size_t newColumns);

    void fill(const T &orig);
    template <typename TR> void copyArea(size_t dest_row, size_t dest_col,
                                         size_t row_count, size_t col_count, matrix<TR> &src,
                                         size_t src_row, size_t src_col);
    template <typename TR> void assign(const matrix<TR> &M);

    T& operator()(size_t r, size_t c);
    const T& operator()(size_t r, size_t c) const;

    template <typename TR> matrix& operator+=(const matrix<TR> &rhs);
    template <typename TR> matrix& operator-=(const matrix<TR> &rhs);
    template <typename TR> matrix& operator/=(const matrix<TR> &rhs);
    template <typename TR> matrix& operator+=(const TR &rhs);
    template <typename TR> matrix& operator-=(const TR &rhs);
    template <typename TR> matrix& operator*=(const TR &rhs);
    template <typename TR> matrix& operator/=(const TR &rhs);

    template <typename TR> void elementwiseMult(const matrix<TR> &rhs);
    matrix& transp();
    T det();
    T trace();
    void leverrier(matrix &pol);
    void mainMinors(matrix<T> &poly, T (deter)(matrix<T>&));


    template <typename T1, typename T2>
    static void multiply(matrix &res, matrix<T1> &lhs, matrix<T2> &rhs);

    template <typename T1, typename T2>
    static void kronekerProduct(matrix &res, matrix<T1> &lhs, matrix<T2> &rhs);

    static matrix<T>& eyeMatrix(size_t n);


private:
    size_t rows;    /*!< Количество строк матрицы. */
    size_t columns; /*!< Количество столбцов матрицы. */
    T **matr_ptr;   /*!< Указатель на массив с указателями на строки матрицы */

    void destroy();
    template <typename TR> void copy_elems(const matrix<TR>&);
    T** allocate_elems(size_t r, size_t c);

    T _recurse_det(size_t order);
    static bool* rows_flags;        /*!< Массив флагов вычеркиваемых строк. */
    static bool* columns_flags;     /*!< Массив флагов вычеркиваемых столбцов. */
};

template <typename T> void formControllabilityMatrix(matrix<T> &res, matrix<T> &A, matrix<T> &B);
template <typename T> std::ostream& operator <<(std::ostream &os, const matrix<T> &M);

}

#include "matrix.cpp"


#endif // MATRIX_H
