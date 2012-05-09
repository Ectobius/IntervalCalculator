#ifndef INTERVAL_EXT_H
#define INTERVAL_EXT_H

#include "boost/numeric/interval.hpp"
#include <iostream>
namespace int_calc
{
template <class T, class P> std::ostream& operator<<(std::ostream&,
                                                     const boost::numeric::interval<T, P>&);
}
#include "interval_ext.cpp"

#endif // INTERVAL_EXT_H
