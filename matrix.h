#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>
#include <stdexcept>

namespace int_calc
{
class size_mismatch : public std::runtime_error
{
public:
    size_mismatch(const std::string &what_arg) :
        std::runtime_error(what_arg)
    {
    }
};

template <typename T> class matrix
{
    template <typename T2> friend class matrix;
public:
    matrix();
    matrix(size_t r, size_t c);
    matrix(const matrix&);

    matrix& operator=(const matrix&);

    ~matrix();

    const size_t getRows() const { return rows; }
    const size_t getColumns() const { return columns; }

    void fill(const T&);
    template <typename TR> void copyArea(size_t dest_row, size_t dest_col,
                                         size_t row_count, size_t col_count, matrix<TR> &src,
                                         size_t src_row, size_t src_col);
    template <typename TR> void assign(const matrix<TR>&);

    T& operator()(size_t r, size_t c);
    const T& operator()(size_t r, size_t c) const;

    template <typename TR> matrix& operator+=(const matrix<TR>&);
    template <typename TR> matrix& operator-=(const matrix<TR>&);
    template <typename TR> matrix& operator/=(const matrix<TR>&);
    template <typename TR> matrix& operator+=(const TR&);
    template <typename TR> matrix& operator-=(const TR&);
    template <typename TR> matrix& operator*=(const TR&);
    template <typename TR> matrix& operator/=(const TR&);


    template <typename T1, typename T2>
    static void multiply(matrix&, matrix<T1>&, matrix<T2>&);

private:
    size_t rows;
    size_t columns;
    T **matr_ptr;

    void destroy();
    template <typename TR> void copy_elems(const matrix<TR>&);
};

template <typename T> void print_matr(const matrix<T> &M);

}

#include "matrix.cpp"


#endif // MATRIX_H
