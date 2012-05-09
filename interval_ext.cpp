#ifndef INTERVAL_EXT_CPP
#define INTERVAL_EXT_CPP

#include "interval_ext.h"
namespace int_calc
{

template <class T, class P> std::ostream& operator<<(std::ostream& os,
                                                     const boost::numeric::interval<T, P>& interv)
{
    os << "[" << interv.lower() << "; " << interv.upper() << "]";
    return os;
}

}

#endif // INTERVAL_EXT_CPP
