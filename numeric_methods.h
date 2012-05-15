#ifndef NUMERIC_METHODS_H
#define NUMERIC_METHODS_H

namespace int_calc
{
namespace num_methods
{

int re_split(int dim, double *A, double *b, double *x, double eps, double tau, int iterLim);
int subdiff(int dim, double *A, double *b, double *x, double eps, double tau, int iterLim);

}
}

#endif // NUMERIC_METHODS_H
