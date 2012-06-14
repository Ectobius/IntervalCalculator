#ifndef INTERVAL_EXT_CPP
#define INTERVAL_EXT_CPP

#include "interval_ext.h"
namespace int_calc
{

/*!
  \brief Оператор вывода.
  \tparam T Тип концов интервала.
  \tparam P Тип политики округления интервала.
  \param os Ссылка на поток вывода.
  \param interv Выводимый интервал.
  \return Ссылка на левый операнд.
 */
template <class T, class P> std::ostream& operator<<(std::ostream& os,
                                                     const boost::numeric::interval<T, P>& interv)
{
    os << "[" << interv.lower() << "; " << interv.upper() << "]";
    return os;
}

/*!
  \brief Оператор меньше.
  \tparam T Тип концов интервала.
  \tparam P Тип политики округления интервала.
  \param lhs Левый операнд.
  \param rhs Правый операнд.
  \return Результат сравнения.
 */
template <class T, class P> bool operator <(const boost::numeric::interval<T, P> &lhs,
                                            const boost::numeric::interval<T, P> &rhs)
{
    return lhs.lower() < rhs.lower() &&
            lhs.upper() < rhs.upper();
}

/*!
  \brief Оператор больше.
  \tparam T Тип концов интервала.
  \tparam P Тип политики округления интервала.
  \param lhs Левый операнд.
  \param rhs Правый операнд.
  \return Результат сравнения.
 */
template <class T, class P> bool operator >(const boost::numeric::interval<T, P> &lhs,
                                            const boost::numeric::interval<T, P> &rhs)
{
    return lhs.lower() > rhs.lower() &&
            lhs.upper() > rhs.upper();
}

}

#endif // INTERVAL_EXT_CPP
