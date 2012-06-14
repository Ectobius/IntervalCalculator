/*!
  \file interval_ext.h

  Определяет некоторые функции над boost::numeric::interval,
  которых нет в boost или определение которых в boost не
  является подходящим.
 */

#ifndef INTERVAL_EXT_H
#define INTERVAL_EXT_H

#include "boost/numeric/interval.hpp"
#include <iostream>

/*!
  Пространство имен, содержащее классы ядра приложения.
 */
namespace int_calc
{
template <class T, class P> std::ostream& operator<<(std::ostream&,
                                                     const boost::numeric::interval<T, P>&);

template <class T, class P> bool operator <(const boost::numeric::interval<T, P> &lhs,
                                            const boost::numeric::interval<T, P> &rhs);

template <class T, class P> bool operator >(const boost::numeric::interval<T, P> &lhs,
                                            const boost::numeric::interval<T, P> &rhs);

}
#include "interval_ext.cpp"

#endif // INTERVAL_EXT_H
