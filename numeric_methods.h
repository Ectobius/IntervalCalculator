#ifndef NUMERIC_METHODS_H
#define NUMERIC_METHODS_H

#include "matrix.h"
#include "object_storage.h"

namespace int_calc
{
namespace num_methods
{

const double epsilon = 0.00000001;

int re_split(int dim, double *A, double *b, double *x, double eps, double tau, int iterLim);
int subdiff(int dim, double *A, double *b, double *x, double eps, double tau, int iterLim);
double determinant(matrix<double> &matr);
bool isStablePolynom(matrix<double> &poly);
bool haritonovCritery(matrix<d_interval> &poly);

}
}

#endif // NUMERIC_METHODS_H
