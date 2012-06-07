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
bool inverseMatrix(matrix<double> &matr, matrix<double> &res);
matrix<double>& designFeedbackControl(matrix<d_interval> &A,
                                          matrix<d_interval> &b, matrix<d_interval> &D);

void intervalEuler(matrix<d_interval> &A, matrix<d_interval> &x0, double h,
                   double t0, double t1, vector< matrix<d_interval> > &res);

void numericEuler(matrix<double> &A, matrix<double> &x0, double h,
                  double t0, double t1, vector< matrix<double> > &res);

matrix<double>* randMatrix(size_t n, size_t m);

}
}

#endif // NUMERIC_METHODS_H
