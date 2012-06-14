#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <vector>
#include "matrix.h"


using namespace std;

namespace int_calc
{

/*!
  \brief Конструктор по умолчанию.
 */
template <typename T> matrix<T>::matrix() :
    rows(0),
    columns(0),
    matr_ptr(0)
{

}

/*!
  \brief Конструктор, создающий матрицу заданного размера/
  \param r Количество строк матрицы.
  \param c Количество столбцов матрицы.
 */
template <typename T> matrix<T>::matrix(size_t r, size_t c) :
    rows(r),
    columns(c),
    matr_ptr(0)
{
    matr_ptr = new T*[rows];
    for(size_t i = 0; i != rows; ++i)
    {
        matr_ptr[i] = new T[columns];
    }
}

/*!
  \brief Метод, освобождающий память, занятую матрицей/
 */
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

/*!
  \brief Метод выделяющий память под элементы матрицы и копирующий их из другой матрицы.
  \tparam TR Тип элементов копируемой матрицы.
  \param M Копируемая матрица.
 */
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

/*!
  \brief Выделяет память под элементы матрицы заданного размера.
  \param r Количество строк.
  \param c Количество столбцов.
  \return Указатель на выделенную область памяти.
 */
template <typename T> T** matrix<T>::allocate_elems(size_t r, size_t c)
{
    T** res_ptr = new T*[r];
    for(size_t i = 0; i != r; ++i)
    {
        res_ptr[i] = new T[c];
    }

    return res_ptr;
}

/*!
  \brief Конструктор копирования.
  \param M Копируемая матрица.
 */
template <typename T> matrix<T>::matrix(const matrix &M) :
    rows(M.rows),
    columns(M.columns),
    matr_ptr(0)
{
    copy_elems(M);
}

/*!
  \brief Оператор присваивания.
  \param M Присваиваемая матрица.
  \return Ссылка на левый операнд.
 */
template <typename T> matrix<T>&
matrix<T>::operator=(const matrix<T> &M)
{
    destroy();
    copy_elems(M);
    return *this;
}

/*!
  \brief Деструктор.
 */
template <typename T> matrix<T>::~matrix()
{
    destroy();
}

/*!
  \brief Изменяет размеры матрицы.
  \param newRows Новое количество строк.
  \param newColumns Новое количество столбцов.
 */
template <typename T> void matrix<T>::resize(size_t newRows, size_t newColumns)
{
    destroy();
    matr_ptr = allocate_elems(newRows, newColumns);
    rows = newRows;
    columns = newColumns;
}

/*!
  \brief Присваивает заданное значение всем элементам матрицы.
  \param orig Значение, присваимаемое элементам матрицы.
 */
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

/*!
  \brief Присвоение матрицы совместимого типа.

  Присваивает данной матрице другую матрицу с элементами,
  тип которых допускает присвоение типу элементов данной матрицы.

  \tparam TR Тип элементов присваиваемой матрицы.
  \param M Присваиваемая матрица.
 */
template <typename T> template <typename TR> void matrix<T>::assign(const matrix<TR> &M)
{
    destroy();
    copy_elems(M);
}

/*!
  \brief Копирует заданную область из другой матрицы.
  \tparam TR Тип элементов матрицы, откуда копируется область.
  \param dest_row Индекс первой строки области, куда нужно скопировать.
  \param dest_row Индекс первого столбца области, куда нужно скопировать.
  \param row_count Количество копируемых строк.
  \param col_count Количество копируемых столбцов.
  \param src Матрица, из которой копируется область.
  \param dest_row Индекс первой строки копируемой области.
  \param dest_row Индекс первого столбца копируемой области.
 */
template <typename T> template <typename TR> void matrix<T>::copyArea(size_t dest_row, size_t dest_col,
                                     size_t row_count, size_t col_count, matrix<TR> &src,
                                     size_t src_row, size_t src_col)
{
    if(dest_row + row_count > rows ||
            dest_col + col_count > columns ||
            src_row + row_count > src.getRows() ||
            src_col + col_count > src.getColumns())
    {
        throw runtime_error("Specified area is out of range");
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

/*!
  \brief Возвращает ссылку на элемент матрицы.
  \param r Номер строки.
  \param c Номер столбца.
  \return Ссылка на элемент матрицы.
 */
template <typename T> T& matrix<T>::operator()(size_t r, size_t c)
{
    if(r >= rows || c >= columns)
    {
        throw runtime_error("Index is out of range");
    }
    return matr_ptr[r][c];
}

/*!
  \brief Константный метод, возвращающий константную ссылку на элемент матрицы.
  \param r Номер строки.
  \param c Номер столбца.
  \return Константная ссылка на элемент матрицы.
 */
template <typename T> const T& matrix<T>::operator()(size_t r, size_t c) const
{
    if(r >= rows || c >= columns)
    {
        throw runtime_error("Index is out of range");
    }
    return matr_ptr[r][c];
}

/*!
  \brief Составной оператор присвоения для сложения матриц.
  \tparam TR Тип элементов правого операнда.
  \param rhs Правый операнд оператора.
  \return Ссылка на левый операнд.
 */
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

/*!
  \brief Составной оператор присвоения для вычитания матриц.
  \tparam TR Тип элементов правого операнда.
  \param rhs Правый операнд оператора.
  \return Ссылка на левый операнд.
 */
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

/*!
  \brief Составной оператор присвоения для деления матрицы на заданное значение.

  Правый операнд должен быть размера 1x1. Элементы левого операнда делятся
  на единственный элемент правого.

  \tparam TR Тип элементов правого операнда.
  \param rhs Правый операнд оператора.
  \return Ссылка на левый операнд.
 */
template <typename T> template <typename TR> matrix<T>& matrix<T>::operator/=(const matrix<TR> &rhs)
{
    if(rhs.getRows() != 1 || rhs.getColumns() != 1)
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    (*this) /= rhs(0, 0);

    return *this;
}

/*!
  \brief Составной оператор присвоения для сложения матрицы с заданным значением.
  \tparam TR Тип правого операнда.
  \param rhs Значение, прибавляемое к элементам левого операнда.
  \return Ссылка на левый операнд.
 */
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

/*!
  \brief Составной оператор присвоения для вычитания заданного значения из матрицы.
  \tparam TR Тип правого операнда.
  \param rhs Значение, вычитаемое из элементов левого операнда.
  \return Ссылка на левый операнд.
 */
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

/*!
  \brief Составной оператор присвоения для умножения матрицы на заданное значение.
  \tparam TR Тип правого операнда.
  \param rhs Значение, на которое умножается матрица.
  \return Ссылка на левый операнд.
 */
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

/*!
  \brief Составной оператор присвоения для деления матрицы на заданное значение.
  \tparam TR Тип правого операнда.
  \param rhs Значение, на которое делится матрица.
  \return Ссылка на левый операнд.
 */
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

/*!
  \brief Поэлементное произведение матриц.
  \tparam TR Тип элементов матрицы-аргумента.
  \param rhs Матрица, на которую поэлементно умножается данная матрица.
 */
template <typename T> template <typename TR> void matrix<T>::elementwiseMult(const matrix<TR> &rhs)
{
    if(rows != rhs.rows || columns != rhs.columns)
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    for(size_t i = 0; i != rows; ++i)
    {
        for(size_t j = 0; j != columns; ++j)
        {
            matr_ptr[i][j] *= rhs.matr_ptr[i][j];
        }
    }
}

/*!
  \brief Транспонирование матрицы.

  Транспонирует матрицу и возвращает ссылку на нее.

  \return Ссылка на данную матрицу после транспонирования.
 */
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


/*!
  \brief Вычисляет определитель разложением по строке.
  \return Значение определителя.
 */
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

    T result = _recurse_det(rows);
    delete rows_flags;
    delete columns_flags;
    rows_flags = 0;
    columns_flags = 0;
    return result;
}

/*!
  \brief Рекурсивный метод вычисления дополнительного минора разложением по строке.
  \param order Текущий порядок дополнительного минора.
  \return Значение дополнительного минора.
 */
template <typename T> T matrix<T>::_recurse_det(size_t order)
{
    T epsT = T(1e-8);
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
        size_t chosenRow = initRow;
        size_t maxZeroCount = 0;
        for (size_t i = initRow; i != rows; ++i)
        {
            size_t zeroCount = 0;
            if (!rows_flags[i])
            {
                for (size_t j = initCol; j != columns; ++j)
                {
                    if (!columns_flags[j] &&
                            matr_ptr[i][j] < epsT && matr_ptr[i][j] > -epsT)
                    {
                        ++zeroCount;
                    }
                }
                if (zeroCount > maxZeroCount)
                {
                    maxZeroCount = zeroCount;
                    chosenRow = i;
                }
            }
        }

        rows_flags[chosenRow] = true;
        for(size_t j = initCol; j != columns; ++j)
        {
            if(!columns_flags[j] &&
                    (matr_ptr[chosenRow][j] < -epsT || matr_ptr[chosenRow][j] > epsT))
            {
                columns_flags[j] = true;
                curDet = _recurse_det(order - 1);
                columns_flags[j] = false;
                if((initRow + j) % 2)
                {
                    result -= matr_ptr[chosenRow][j] * curDet;
                }
                else
                {
                    result += matr_ptr[chosenRow][j] * curDet;
                }
            }
        }
        rows_flags[chosenRow] = false;
    }
    return result;
}

/*!
  \brief Вычисляет след матрицы.
  \return След матрицы.
 */
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

/*!
  \brief Вычисляет характеристический полином матрицы методом Леверье.
  \param pol Выходной параметр, куда будет записан полином.
 */
template <typename T> void matrix<T>::leverrier(matrix<T> &pol)
{
    if(rows != columns)
    {
        throw size_mismatch("It is not a square matrix");
    }
    pol.resize(1, rows + 1);
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

/*!
  \brief Рекурсивный метод вычисления суммы главных миноров матрицы.
  \param matr Матрица, сумма главных миноров которой вычисляется.
  \param flags Массив флагов, отмечающих строки и столбцы минора.
  \param k Порядок вычисляемого минора.
  \param cur_k Количество строк, которые нужно еще отметить для полуения минора порядка k.
  \param pos Строка и столбец матрицы, начиная с которой можно выбирать отмечаемые строки и столбцы.
  \param deter Функция вычисления определителя матрицы.
  \return Текущее значение суммы.
 */
template <typename T> T _mainMinorsSum(
    matrix<T> &matr, bool *flags, int k, int cur_k, int pos, T (deter)(matrix<T>&))
{
    if (cur_k == 0)
    {
        matrix<T> minor(k, k);
        for (int i = 0, row = 0; i != matr.getRows(); ++i)
        {
            if (flags[i])
            {
                for (int j = 0, col = 0; j != matr.getColumns(); ++j)
                {
                    if (flags[j])
                    {
                        minor(row, col) = matr(i, j);
                        ++col;
                    }
                }
                ++row;
            }
        }
        return deter(minor);
    }

    T sum(0);
    for (int i = pos; i <= matr.getRows() - cur_k; ++i)
    {
        flags[i] = true;
        sum += _mainMinorsSum(matr, flags, k, cur_k - 1, i + 1, deter);
        flags[i] = false;
    }

    return sum;
}

/*!
  \brief Вычисляет характеристический полином матрицы методом главных миноров.
  \param poly Выходной параметр, куда будет записан полином.
  \param deter Функция вычисления определителя матрицы.
 */
template <typename T> void matrix<T>::mainMinors(matrix<T> &poly, T (deter)(matrix<T>&))
{
    if (this->getRows() != this->getColumns())
    {
        throw size_mismatch("Expected square matrix");
    }

    size_t n = this->getRows();
    if (poly.getColumns() != n + 1 || poly.getRows() != 1)
    {
        poly.resize(1, n + 1);
    }

    poly(0, 0) = 1;
    poly(0, 1) = -this->trace();

    bool *flags = new bool[n];
    for (int i = 2; i <= n; ++i)
    {
        for (int j = 0; j != n; ++j)
            flags[j] = false;
        poly(0, i) = _mainMinorsSum(*this, flags, i, i, 0, deter);
        if (i % 2)
        {
            poly(0, i) = -poly(0, i);
        }
    }
    delete [] flags;
}

/*!
  \brief Умножение матриц.
  \tparam T1 Тип элементов левого операнда.
  \tparam T2 Тип элементов правого операнда.
  \param res Выходной параметр, куда будет записан результат умножения.
  \param matr1 Левый операнд.
  \param matr2 Правый операнд.
 */
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
            res.resize(matr1.getRows(), matr2.getColumns());
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

/*!
  \brief Кронекерово произведение матриц.
  \tparam T1 Тип элементов левого операнда.
  \tparam T2 Тип элементов правого операнда.
  \param res Выходной параметр, куда будет записано кронекерово произведение.
  \param lhs Левый операнд.
  \param rhs Правый операнд.
 */
template <typename T> template <typename T1, typename T2>
void matrix<T>::kronekerProduct(matrix<T> &res, matrix<T1> &lhs, matrix<T2> &rhs)
{
    if (res.getRows() != lhs.getRows() * rhs.getRows() ||
            res.getColumns() != lhs.getColumns() * rhs.getColumns())
    {
        res.resize(lhs.getRows() * rhs.getRows(),
                   lhs.getColumns() * rhs.getColumns());
    }

    matrix<T2> copyRhs(rhs);

    for (int i = 0; i != lhs.getRows(); ++i)
    {
        for (int j = 0; j != lhs.getColumns(); ++j)
        {
            copyRhs *= lhs(i, j);
            res.copyArea(i * rhs.getRows(), j * rhs.getColumns(),
                         rhs.getRows(), rhs.getColumns(),
                         copyRhs, 0, 0);
            copyRhs = rhs;
        }
    }

    return;
}

/*!
  \brief Единичная матрица порядка n.
  \param n Порядок матрицы.
  \return Ссылка на созданную матрицу.
 */
template <typename T> matrix<T>& matrix<T>::eyeMatrix(size_t n)
{
    matrix<T> *res = new matrix<T>(n, n);
    res->fill(T(0));
    for(size_t i = 0; i != n; ++i)
        (*res)(i, i) = T(1);
    return *res;
}

/*!
  \brief Формирование матрицы управляемости для пары матриц (A, B)
  \param res Выходной параметр, куда будет записана матрица управляемости.
  \param A Квадратная матрица nxn.
  \param B Матрица nxm.
 */
template <typename T> void formControllabilityMatrix(matrix<T> &res, matrix<T> &A, matrix<T> &B)
{
    if (A.getRows() != A.getColumns())
    {
        throw size_mismatch("A must be a square matrix");
    }
    if (B.getRows() != A.getRows())
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    int n = A.getRows(),
            r = B.getColumns();

    if (res.getRows() != n ||
            res.getColumns() != n * r)
    {
        res = matrix<T>(n, n * r);
    }

    matrix<T> A_n = matrix<T>::eyeMatrix(n), tmp_A_n = A,
            tmp = B;

    for (int i = 0; i < n; ++i)
    {
        res.copyArea(0, i * r, n, r, tmp, 0, 0);
        matrix<T>::multiply(tmp_A_n, A_n, A);
        A_n = tmp_A_n;
        matrix<T>::multiply(tmp, A_n, B);
    }

}

/*!
  \brief Оператор вывода матрицы.
  \param os Ссылка на поток вывода std::ostream
  \param M Ссылка на выводимую матрицу.
  \return Ссылка на левый операнд.
 */
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
