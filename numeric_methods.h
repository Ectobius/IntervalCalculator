/*!
  \file numeric_methods.h

  Содержит реализации некоторых численных методов.
 */

#ifndef NUMERIC_METHODS_H
#define NUMERIC_METHODS_H

#include "matrix.h"
#include "object_storage.h"


/*!
  Пространство имен, содержащее классы ядра приложения.
 */
namespace int_calc
{

/*!
  Пространство имен, содержащее реализации некоторых численных методов.
 */
namespace num_methods
{

const double epsilon = 0.00000001;

int re_split(int dim, double *A, double *b, double *x, double eps, double tau, int iterLim);
int subdiff(int dim, double *A, double *b, double *x, double eps, double tau, int iterLim);
double determinant(matrix<double> &matr);
bool isStablePolynom(matrix<double> &poly);
bool haritonovCritery(matrix<interval_double> &poly);
bool inverseMatrix(matrix<double> &matr, matrix<double> &res);
matrix<double>& designFeedbackControl(matrix<interval_double> &A,
                                          matrix<interval_double> &b, matrix<interval_double> &D);

void intervalEuler(matrix<interval_double> &A, matrix<interval_double> &x0, double h,
                   double t0, double t1, vector< matrix<interval_double> > &res);

void numericEuler(matrix<double> &A, matrix<double> &x0, double h,
                  double t0, double t1, vector< matrix<double> > &res);

matrix<double>* randMatrix(size_t n, size_t m);


}
}

#endif // NUMERIC_METHODS_H
