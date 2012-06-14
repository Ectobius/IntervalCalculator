/*     Computing formal-algebraic solution to interval linear systems
         in Kaucher arithmetic  by single-step stationary iterative
           method  with  modified  real  splitting  of the matrix,
                      (c) Sergey P. Shary, 1995                             */

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <ctime>

#include "numeric_methods.h"

using namespace boost::numeric;

namespace int_calc
{
namespace num_methods
{

int re_split(int dim, double *A, double *b, double *x, double eps, double tau, int iterLim)
{
    char *Z;
    int   ier, *ip, i, j, k, m, ni, DIM;
    double   g0, g1, h0, h1, p, q, s0, s1, t0, t1, u0, u1, v0, v1;
    double   *G, *d, *H, *x1, *xx;


    /*reading input data, forming the "middle" matrix G for the given system*/

    DIM = 2 * dim;
    ip = (int*)calloc(DIM, sizeof(int));
    G = (double*)calloc(DIM*DIM, sizeof(double));
    x1 = (double*)calloc(DIM, sizeof(double));
    xx = (double*)calloc(DIM, sizeof(double));
    H = (double*)calloc(dim*DIM, sizeof(double));
    Z = (char*)calloc(dim*DIM, sizeof(char));
    d = (double*)calloc(DIM, sizeof(double));

    if( ip==NULL||G==NULL||x1==NULL||xx==NULL||H==NULL||Z==NULL||d==NULL )
    {

    }

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++)
        {
            u0 = *(A+i*DIM+2*j);
            u1 = *(A+i*DIM+2*j+1);
            *(H+i*DIM+2*j) = u0;
            *(H+i*DIM+2*j+1) = u1;
            p=0.5*(u0+u1);
            if( p>=0. )
            {
                *(G+i*DIM+j)=p; *(G+(i+dim)*DIM+j+dim)=p;
                *(G+(i+dim)*DIM+j)=0; *(G+i*DIM+j+dim)=0;
            }
            else
            {
                *(G+i*DIM+j)=0; *(G+(i+dim)*DIM+j+dim)=0;
                *(G+(i+dim)*DIM+j)=p; *(G+i*DIM+j+dim)=p;
            }
        }

    for(i = 0; i < dim; i++)
    {
        v0 = *(b + 2 * i);
        v1 = *(b + 2 * i + 1);
        *(xx+i)=v0;  *(xx+i+dim)=v1;
        *(d+2*i)=v0;  *(d+2*i+1)=v1;
    }


       /*   solving the immersed "middle" system,
                              we find a starting approximation    */

    ier=0;
    *(ip+DIM-1)=1;
    for( k=0; k<DIM-1; k++ )
      { m=k; p=fabs(*(G+k*DIM+k));
        for( i=k+1; i<DIM; i++ )
          { q=fabs(*(G+i*DIM+k)); if( p<q ) { m=i; p=q; } }
          *(ip+k)=m; p=*(G+m*DIM+k);
          if( m!=k ) { *(ip+DIM-1)=-*(ip+DIM-1);
                       *(G+m*DIM+k)=*(G+k*DIM+k);
                       *(G+k*DIM+k)=p;
                     }
          if( p==0 ) { ier=k; *(ip+DIM-1)=0; break; }
          p=1./p;
          for( i=k+1; i<DIM; i++ ) *(G+i*DIM+k)*=-p;
          for( j=k+1; j<DIM; j++ )
              { p=*(G+m*DIM+j); *(G+m*DIM+j)=*(G+k*DIM+j); *(G+k*DIM+j)=p;
                if( p!=0 ) for( i=k+1; i<DIM; i++ ) *(G+i*DIM+j)+=*(G+i*DIM+k)*p;
              }
      }

    if(ier!=0 || *(G+DIM*DIM-1)==0)  //!!!!!!!!!!!!!!!
    {
//        cprintf("\n\r   Unfortunately, the matrix of the system is i-singular!");
//        cprintf("\n\r   Try to change its elements a little.\n");
//        goto final;
        return 0;
    }

    for( k=0; k<DIM-1; k++ )
     {  m=*(ip+k); q=*(xx+m); *(xx+m)=*(xx+k); *(xx+k)=q;
        for( i=k+1; i<DIM; i++ ) *(xx+i)+=*(G+i*DIM+k)*q;
     }

    for( j=0; j<DIM-1; j++ )
      {   k=DIM-j-1;  *(xx+k)/=*(G+k*DIM+k);  q=-*(xx+k);
        for( i=0; i<k; i++ ) *(xx+i)+=*(G+i*DIM+k)*q;
      }
                 *xx/=*G;


       /*      forming the splitting of the interval matrix     */

    for( i=0; i<dim; i++ )
    for( j=0; j<dim; j++ )
      { u0=*(H+DIM*i+2*j); u1=*(H+DIM*i+2*j+1);
        p=0; *(Z+DIM*i+2*j)=(u0<u1)? 0:1;
         if( u0<=0 && u1<=0 ) p=(u0<u1)? u0:u1;
         if( u0>=0 && u1>=0 ) p=(u0>u1)? u0:u1;
        *(H+DIM*i+2*j)=u0-p; *(H+DIM*i+2*j+1)=u1-p;
        *(Z+DIM*i+2*j+1)=(p==0)? 0:1;
        if( p>=0. )
           { *(G+i*DIM+j)=p; *(G+(i+dim)*DIM+j+dim)=p;
             *(G+(i+dim)*DIM+j)=0; *(G+i*DIM+j+dim)=0;
           }
        else
           { *(G+i*DIM+j)=0; *(G+(i+dim)*DIM+j+dim)=0;
             *(G+(i+dim)*DIM+j)=p; *(G+i*DIM+j+dim)=p;
           }
      }

          /*    finding triangular decomposition of the splitted matrix G    */

         ier=0;     *(ip+DIM-1)=1;
    for( k=0; k<DIM-1; k++ )
      { m=k; p=fabs(*(G+k*DIM+k));
        for( i=k+1; i<DIM; i++ )
          { q=fabs(*(G+i*DIM+k)); if( p<q ) { m=i; p=q; } }
          *(ip+k)=m; p=*(G+m*DIM+k);
          if( m!=k ) { *(ip+DIM-1)=-*(ip+DIM-1);
                       *(G+m*DIM+k)=*(G+k*DIM+k);
                       *(G+k*DIM+k)=p;
                     }
          if( p==0 ) { ier=k; *(ip+DIM-1)=0; break; }
          p=1./p;
          for( i=k+1; i<DIM; i++ ) *(G+i*DIM+k)*=-p;
          for( j=k+1; j<DIM; j++ )
              { p=*(G+m*DIM+j); *(G+m*DIM+j)=*(G+k*DIM+j); *(G+k*DIM+j)=p;
                if( p!=0 ) for( i=k+1; i<DIM; i++ ) *(G+i*DIM+j)+=*(G+i*DIM+k)*p;
              }
      }

    if(ier!=0 || *(G+DIM*DIM-1)==0) //!!!!!!!!!!!!!!!!
    {
//        cprintf("\n\r   Unfortunately, the matrix of the system is i-singular!");
//        cprintf("\n\r   Try to change its elements a little.\n");
//        goto final;
        return 0;
    }


       /*    launching iterations   */
    ni=0;

    do {     ni++;   for( i=0; i<DIM; i++ ) *(x1+i)=*(xx+i);

          for( i=0; i<dim; i++ )
           {    s0=*(d+2*i);   s1=*(d+2*i+1);
              for( j=0; j<dim; j++ )
               { g0=*(H+i*DIM+2*j); g1=*(H+i*DIM+2*j+1); h0=*(x1+j); h1=*(x1+j+dim);

                 if( *(Z+DIM*i+2*j+1) ) { p=h0; h0=h1; h1=p; }
                 if( h0>=0 && h1>=0 )  m=1;
                 if( h0<=0 && h1>=0 )  m=2;
                 if( h0<=0 && h1<=0 )  m=3;
                 if( h0>=0 && h1<=0 )  m=4;

                 switch ( *(Z+DIM*i+2*j)*4+m )
                    { case 1: t0=g0*h1; t1=g1*h1; break;
                      case 2: p=g0*h1; q=g1*h0; t0=(p<q)?p:q;
                              p=g0*h0; q=g1*h1; t1=(p>q)?p:q; break;
                      case 3: t0=g1*h0; t1=g0*h0; break;
                      case 4: t0=0; t1=0; break;
                      case 5: t0=g0*h0; t1=g1*h0; break;
                      case 6: t0=0; t1=0; break;
                      case 7: t0=g1*h1; t1=g0*h1; break;
                      case 8: p=g0*h0; q=g1*h1; t0=(p>q)?p:q;
                              p=g0*h1; q=g1*h0; t1=(p<q)?p:q; break;
                    }
                       s0-=t0;       s1-=t1;
               }
                  *(xx+i)=s0;     *(xx+i+dim)=s1;
            }

            /*      Solving a linear system with the matrix G.
                         Its triangular decomposition has been already found.   */

          for( k=0; k<DIM-1; k++ )
            {  m=*(ip+k); q=*(xx+m); *(xx+m)=*(xx+k); *(xx+k)=q;
              for( i=k+1; i<DIM; i++ ) *(xx+i)+=*(G+i*DIM+k)*q;
            }

          for( j=0; j<DIM-1; j++ )
            {  k=DIM-j-1;  *(xx+k)/=*(G+k*DIM+k);  q=-*(xx+k);
              for( i=0; i<k; i++ ) *(xx+i)+=*(G+i*DIM+k)*q;
            }
                       *xx/=*G;

            /*    computing relative improvement of the current
                                            approximation to the solution    */

         p=q=0;
         for( i=0; i<DIM; i++ ) { q+=fabs(*(xx+i)); p+=fabs(*(xx+i)-*(x1+i)); }
         if( q==0 ) q=1;

       } while ( p/q>eps && ni<iterLim );


    for( i=0; i<dim; i++ )
    {
        u0=*(xx+i);
        u1=*(xx+i+dim);
        *(x + 2 * i) = u0;
        *(x + 2 * i + 1) = u1;
//           if( u0<=u1 )  cprintf("     ->");
//           else  cprintf("     <-");
    }

    return 1;
}

char solver(int DIM, int *ip, double *F, double *xx)

/*        solving the point linear system
                     with the matrix F  by Gaussian elimination    */

{     int   ier, i, j, k, m;
   double   p, q;

       ier=0;     *(ip+DIM-1)=1;
  for( k=0; k<DIM-1; k++ )
   {  m=k; p=fabs(*(F+k*DIM+k));
      for( i=k+1; i<DIM; i++ )
          { q=fabs(*(F+i*DIM+k)); if( p<q ) { m=i; p=q; } }
      *(ip+k)=m; p=*(F+m*DIM+k);
      if( m!=k ) { *(ip+DIM-1)=-*(ip+DIM-1);
                   *(F+m*DIM+k)=*(F+k*DIM+k);
                   *(F+k*DIM+k)=p;
                 }
      if( p==0 ) { ier=k; *(ip+DIM-1)=0; break; }
      p=1./p;
      for( i=k+1; i<DIM; i++ ) *(F+i*DIM+k)*=-p;
      for( j=k+1; j<DIM; j++ )
          { p=*(F+m*DIM+j); *(F+m*DIM+j)=*(F+k*DIM+j); *(F+k*DIM+j)=p;
            if( p!=0 ) for( i=k+1; i<DIM; i++ ) *(F+i*DIM+j)+=*(F+i*DIM+k)*p;
          }
   }

  if(ier!=0 || *(F+DIM*DIM-1)==0)
  {
      return 1;
  }

  for( k=0; k<DIM-1; k++ )
   {  m=*(ip+k); q=*(xx+m); *(xx+m)=*(xx+k); *(xx+k)=q;
     for( i=k+1; i<DIM; i++ ) *(xx+i)+=*(F+i*DIM+k)*q;
   }

  for( j=0; j<DIM-1; j++ )
    {   k=DIM-j-1;  *(xx+k)/=*(F+k*DIM+k);  q=-*(xx+k);
      for( i=0; i<k; i++ ) *(xx+i)+=*(F+i*DIM+k)*q;
    }
    *xx/=*F;  return 0;
}

/*      Computing  formal-algebraic solution  to interval linear systems
          in  Kaucher  arithmetic  by  subdifferential  Newton method
                    with a special starting approximation,
           (c) Sergey Shary, 1995                                            */
int subdiff(int dim, double *A, double *b, double *x, double eps, double tau, int iterLim)
{
    int   i, j, l, m, ni;
    int   DIM, *ip;
    double   *C, *d, *F, *x1, *xx;
    double   g0, g1, h0, h1, p, q, r, s0, s1, t0, t1, u0, u1, v0, v1;

      /*  Forming the "middle" point matrix F
                                   of the interval system under solution     */

    DIM=2*dim;
    ip = (int*)calloc(DIM,sizeof(int));
    F = (double*)calloc(DIM*DIM,sizeof(double));
    x1 = (double*)calloc(DIM,sizeof(double));
    xx = (double*)calloc(DIM,sizeof(double));
    C = (double*)calloc(dim*DIM,sizeof(double));
    d = (double*)calloc(DIM,sizeof(double));

    if( ip==NULL || F==NULL || x1==NULL || xx==NULL || C==NULL || d==NULL )
    {
    }

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++)
        {
            u0 = *(A + i * DIM + 2 * j);
            u1 = *(A + i * DIM + 2 * j + 1);
            *(C+i*DIM+2*j)=u0; *(C+i*DIM+2*j+1)=u1;
            p=0.5*(u0+u1);
            if( p>=0. )
            {
                *(F+i*DIM+j)=p; *(F+(i+dim)*DIM+j+dim)=p;
                *(F+(i+dim)*DIM+j)=0; *(F+i*DIM+j+dim)=0;
            }
            else
            {
                *(F+i*DIM+j)=0; *(F+(i+dim)*DIM+j+dim)=0;
                *(F+(i+dim)*DIM+j)=p; *(F+i*DIM+j+dim)=p;
            }
        }

    for(i = 0; i < dim; i++)
    {
        v0 = *(b + 2 * i);
        v1 = *(b + 2 * i + 1);
        *(xx+i)=v0;  *(xx+i+dim)=v1;
        *(d+2*i)=v0;  *(d+2*i+1)=v1;
    }


        /*   solving the embedded "middle" point system,
                          we find the starting approximation of the method  */

    if(solver(DIM , ip, F, xx))
    {
        return 0;
    }
       /*     launching the iteration
                                 of the subdifferential Newton method       */
                 ni=0;

    do {  ni++; r=0;  for( i=0; i<DIM; i++ )
                          {  *(x1+i)=*(xx+i);
                             for( j=0; j<DIM; j++ ) *(F+i*DIM+j)=0;
                          }

    for( i=0; i<dim; i++ )
        {  s0=0;   s1=0;
              for( j=0; j<dim; j++ )
               { g0=*(C+i*DIM+2*j); g1=*(C+i*DIM+2*j+1); h0=*(x1+j); h1=*(x1+j+dim);

                    /*            determining the types of intervals            */
                 if ( g0*g1>0 ) l= (g0>0)? 0:2; else l= (g0<=g1)? 1:3;
                 if ( h0*h1>0 ) m= (h0>0)? 1:3; else m= (h0<=h1)? 2:4;

                    /* computing multiplication in Kaucher arithmetic,
                                            forming the subgradient matrix F    */
                 switch ( 4*l+m ) {
                 case  1: t0=g0*h0; t1=g1*h1;
                          *(F+i*DIM+j)=g0; *(F+(i+dim)*DIM+j+dim)=g1;
                          break;
                 case  2: t0=g1*h0; t1=g1*h1;
                          *(F+i*DIM+j)=g1; *(F+(i+dim)*DIM+j+dim)=g1;
                          break;
                 case  3: t0=g1*h0; t1=g0*h1;
                          *(F+i*DIM+j)=g1; *(F+(i+dim)*DIM+j+dim)=g0;
                          break;
                 case  4: t0=g0*h0; t1=g0*h1;
                          *(F+i*DIM+j)=g0; *(F+(i+dim)*DIM+j+dim)=g0;
                          break;
                 case  5: t0=g0*h1; t1=g1*h1;
                          *(F+i*DIM+j+dim)=g0; *(F+(i+dim)*DIM+j+dim)=g1;
                          break;
                 case  6: u0=g0*h1; v0=g1*h0;  u1=g0*h0; v1=g1*h1;
                          if ( u0<v0 ) { t0=u0; *(F+i*DIM+j+dim)=g0; }
                          else { t0=v0; *(F+i*DIM+j)=g1; }
                          if ( u1>v1 ) { t1=u1; *(F+(i+dim)*DIM+j)=g0; }
                          else { t1=v1; *(F+(i+dim)*DIM+j+dim)=g1; }
                          break;
                 case  7: t0=g1*h0; t1=g0*h0;
                          *(F+i*DIM+j)=g1; *(F+(i+dim)*DIM+j)=g0;
                          break;
                 case  8: t0=0; t1=0;
                          break;
                 case  9: t0=g0*h1; t1=g1*h0;
                          *(F+i*DIM+j+dim)=g0; *(F+(i+dim)*DIM+j)=g1;
                          break;
                 case 10: t0=g0*h1; t1=g0*h0;
                          *(F+i*DIM+j+dim)=g0; *(F+(i+dim)*DIM+j)=g0;
                          break;
                 case 11: t0=g1*h1; t1=g0*h0;
                          *(F+i*DIM+j+dim)=g1; *(F+(i+dim)*DIM+j)=g0;
                          break;
                 case 12: t0=g1*h1; t1=g1*h0;
                          *(F+i*DIM+j+dim)=g1; *(F+(i+dim)*DIM+j)=g1;
                          break;
                 case 13: t0=g0*h0; t1=g1*h0;
                          *(F+i*DIM+j)=g0; *(F+(i+dim)*DIM+j)=g1;
                          break;
                 case 14: t0=0; t1=0;
                          break;
                 case 15: t0=g1*h1; t1=g0*h1;
                          *(F+i*DIM+j+dim)=g1; *(F+(i+dim)*DIM+j+dim)=g0;
                          break;
                 case 16: u0=g0*h0; v0=g1*h1;  u1=g0*h1; v1=g1*h0;
                          if (u0>v0) { t0=u0; *(F+i*DIM+j)=g0; }
                          else { t0=v0; *(F+i*DIM+j+dim)=g1; }
                          if (u1<v1) { t1=u1; *(F+(i+dim)*DIM+j+dim)=g0; }
                          else { t1=v1; *(F+(i+dim)*DIM+j)=g1; }
                          break;  }
                       s0+=t0;       s1+=t1;
               }
                 *(xx+i)=t0=s0-*(d+2*i);   *(xx+i+dim)=t1=s1-*(d+2*i+1);
                 t0=fabs(t0); t1=fabs(t1);     r+= (t0>t1)? t0:t1;
           }

                    /*    solving the (point) linear algebraic
                                    system with the subgradient matrix F   */

         if(solver(DIM, ip, F, xx))
         {
             return 0;
         }

         q=0; for( i=0; i<DIM; i++ )
                  {  *(xx+i)=*(x1+i)-*(xx+i)*tau; q+=fabs(*(xx+i)); }

         if( q==0 ) q=1;

       } while ( r/q>eps && ni<iterLim );


    for( i=0; i<dim; i++ )
    {
        u0=*(xx+i);
        u1=*(xx+i+dim);
        *(x + 2 * i) = u0;
        *(x + 2 * i + 1) = u1;
    }
    return 1;
}


/*!
  \brief Вычисляет определитель числовой матрицы приведением к треугольному виду.
  \param matr Числовая матрица.
  \return Значение определителя.
 */
double determinant(matrix<double> &matr)
{
    if(matr.getRows() != matr.getColumns())
    {
        throw size_mismatch("Expected square matrix");
    }
    matrix<double> tmpMatr = matr;
    size_t n = matr.getRows();
    size_t exchCount = 0;
    double det = 0;

    for(size_t i = 0; i < n - 1; ++i)
    {
        double tmpd = fabs(tmpMatr(i, i));
        if(fabs(tmpMatr(i, i)) < epsilon)
        {
            double maxAbs = 0;
            size_t indMax = 0;
            for(size_t j = i + 1; j < n; ++j)
            {
                if(abs(tmpMatr(j, i)) > maxAbs)
                {
                    maxAbs = abs(tmpMatr(j, i));
                    indMax = j;
                }
            }
            if(maxAbs < epsilon)
            {
                return 0;
            }
            double d = 0;
            for(size_t j = 0; j < n; ++j)
            {
                d = tmpMatr(i, j);
                tmpMatr(i, j) = tmpMatr(indMax, j);
                tmpMatr(indMax, j) = d;
            }
            ++exchCount;
        }
        for(size_t j = i + 1; j < n; ++j)
        {
            double c = -tmpMatr(j, i) / tmpMatr(i, i);
            for(size_t k = i + 1; k < n; ++k)
            {
                tmpMatr(j, k) += c * tmpMatr(i, k);
            }
        }
    }

    det = exchCount % 2 ? -1 : 1;
    for(size_t i = 0; i < n; ++i)
    {
        det *= tmpMatr(i, i);
    }
    return det;
}


/*!
  \brief Проверяет вещественный полином на устойчивость по критерию Гурвица.
  \param poly Исследуемый на устойчивость полином. Матрица-строка,
  коэффициенты по убыванию степеней.
  \return true - полином устойчив, false - не устойчив.
 */
bool isStablePolynom(matrix<double> &poly)
{
    if (poly.getRows() != 1)
    {
        throw size_mismatch("Expected row vector");
    }

    size_t n = poly.getColumns() - 1;
    matrix<double> tmpPoly = poly;

    for (size_t i = 0; i != tmpPoly.getColumns(); ++i)
    {
        tmpPoly(0, i) /= tmpPoly(0, 0);
    }

    matrix<double> gurvitsMatr(n, n);
    gurvitsMatr.fill(0);
    int startIndPoly = 1;
    for (size_t i = 0; i != n; ++i)
    {
        int indPoly = startIndPoly;
        for(size_t j = 0; j != n && indPoly >= 0; ++j, --indPoly)
        {
            if(indPoly <= n)
                gurvitsMatr(i, j) = tmpPoly(0, indPoly);
            else
                gurvitsMatr(i, j) = 0;
        }
        startIndPoly += 2;
    }

    matrix<double> mainMinor;
    double det = 0;
    bool isStable = true;
    for (size_t i = 0; i != n; ++i)
    {
        mainMinor = matrix<double>(i + 1, i + 1);
        mainMinor.copyArea(0, 0, i + 1, i + 1, gurvitsMatr, 0, 0);
        det = determinant(mainMinor);
        if(det <= epsilon)
        {
            isStable = false;
            break;
        }
    }
    return isStable;
}

/*!
  \brief Проверяет интервальный полином на устойчивость по критерию Харитонова.
  \param poly Исследуемый на устойчивость полином. Матрица-строка,
  коэффициенты по убыванию степеней.
  \return true - полином устойчив, false - не устойчив.
 */
bool haritonovCritery(matrix<interval_double> &poly)
{
    int n = poly.getColumns() - 1;
    matrix<double> num_poly(1, n + 1);
    bool isStable = true;

    for (int i = 0; i < 4 && isStable; ++i)
    {
        int magic = i;
        for (int j = n; j >= 0; --j)
        {
            if(magic == 0 || magic == 1)
            {
                num_poly(0, j) = poly(0, j).lower();
            }
            else
            {
                num_poly(0, j) = poly(0, j).upper();
            }
            ++magic;
            magic %= 4;
        }
        if(!isStablePolynom(num_poly))
            isStable = false;
    }

    return isStable;
}

/*!
  \brief Находит обратную матрицу.
  \param matr Числовая матрица.
  \param res Выходной параметр для результата.
  \return true - обратная существует и найдена, false - матрица вырожденная.
 */
bool inverseMatrix(matrix<double> &matr, matrix<double> &res)
{
    if(matr.getRows() != matr.getColumns())
    {
        throw size_mismatch("Expected square matrix");
    }
    matrix<double> tmpMatr = matr;
    size_t n = matr.getRows();
    size_t exchCount = 0;

    vector<int> rowsNumbers(n);
    for (int i = 0; i < n; ++i)
        rowsNumbers[i] = i;

    for (size_t i = 0; i < n - 1; ++i)
    {
        double tmpd = fabs(tmpMatr(i, i));
        if (fabs(tmpMatr(i, i)) < epsilon)
        {
            double maxAbs = 0;
            size_t indMax = 0;
            for (size_t j = i + 1; j < n; ++j)
            {
                if(abs(tmpMatr(j, i)) > maxAbs)
                {
                    maxAbs = abs(tmpMatr(j, i));
                    indMax = j;
                }
            }
            if (maxAbs < epsilon)
            {
                return false;
            }
            double d = 0;
            for (size_t j = i; j < n; ++j)
            {
                d = tmpMatr(i, j);
                tmpMatr(i, j) = tmpMatr(indMax, j);
                tmpMatr(indMax, j) = d;
            }
            ++exchCount;
            rowsNumbers[i] = indMax;
        }
        for (size_t j = i + 1; j < n; ++j)
        {
            double c = -tmpMatr(j, i) / tmpMatr(i, i);
            tmpMatr(j, i) = c;
            for (size_t k = i + 1; k < n; ++k)
            {
                tmpMatr(j, k) += c * tmpMatr(i, k);
            }
        }
    }

    matrix<double> predRes(n, n);

    matrix<double> b(n, 1);
    for (int h = 0; h < n; ++h)
    {
        b.fill(0);
        b(h, 0) = 1;

        for (int i = 0; i < n - 1; ++i)
        {
            if (rowsNumbers[i] != i)
            {
                double d = b(i, 0);
                b(i, 0) = b(rowsNumbers[i], 0);
                b(rowsNumbers[i], 0) = d;
            }
            for (int j = i + 1; j < n; ++j)
            {
                b(j, 0) += tmpMatr(j, i) * b(i, 0);
            }
        }

        for (int i = n - 1; i >= 0; --i)
        {
            predRes(i, h) = b(i, 0) / tmpMatr(i, i);
            for (size_t j = i + 1; j < n; ++j)
            {
                predRes(i, h) -= tmpMatr(i, j) * predRes(j, h) / tmpMatr(i, i);
            }
        }
    }

    if (res.getRows() != n ||
            res.getColumns() != n)
    {
        res = matrix<double>(n, n);
    }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            res(i, j) = predRes(i, j);
        }

    return true;
}

/*!
  \brief Рассчитывает управление в виде обратной связи.
  \param A Квадратная nxn матрица системы.
  \param b Столбец nx1.
  \param D Желаемый полином - матрица 1x(n + 1).
  \return Управление - строка 1xn.
 */
matrix<double>& designFeedbackControl(matrix<interval_double> &A,
                                          matrix<interval_double> &b, matrix<interval_double> &D)
{
    if (A.getRows() != A.getColumns())
    {
        throw size_mismatch("Expected square matrix");
    }
    if (A.getRows() != b.getRows())
    {
        throw size_mismatch("Sizes of matrices do not match");
    }
    if (b.getColumns() != 1)
    {
        throw size_mismatch("Expected column vector");
    }
    if (D.getRows() != 1)
    {
        throw size_mismatch("Expected row vector");
    }
    if (D.getColumns() != A.getColumns() + 1)
    {
        throw size_mismatch("Sizes of matrices do not match");
    }

    int n = A.getRows();  //Порядок системы
    matrix<interval_double> Y =  //Матрица управляемости
            matrix<interval_double>(n, n);

    //Построение матрицы управляемости
    formControllabilityMatrix(A, b, Y);

    matrix<interval_double> charact_poly(1, n + 1);  //Характеристический многочлен

    //Построение характеристического многочлена
    A.leverrier(charact_poly);

    //Проверка ширины коэффициентов
    bool isWider = true;
    for (int i = 1; i <= n && isWider; ++i)
    {
        if (width(charact_poly(0, i)) >= width(D(0, i)))
        {
            isWider = false;
        }
    }

    if (!isWider)
    {
        throw runtime_error("Criterion is not satisfied");
    }

    matrix<interval_double> polyMatr(n, n);  //Матрица на основе коэффициентов хар. полинома
    polyMatr.fill(0);
    for (int i = 0; i < n; ++i)
    {
        for (int j = i; j < n; ++j)
        {
            polyMatr(i, j - i) = -charact_poly(0, n - j - 1);
        }
    }

    matrix<interval_double> P(n, n);

    matrix<interval_double>::multiply(P, Y, polyMatr);

    matrix<interval_double> f(1, n);
    for (int i = 0; i < n; ++i)
    {
        f(0, i) = interval_double(D(0, n - i).lower() - charact_poly(0, n - i).lower(),
                             D(0, n - i).upper() - charact_poly(0, n - i).upper());
    }

    matrix<double> mf(1, n),
            mP(n, n);

    for (int i = 0; i < n; ++i)
    {
        mf(0, i) = median(f(0, i));
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            mP(i, j) = median(P(i, j));
        }
    }

    matrix<double> invMP(n, n);

    inverseMatrix(mP, invMP);

    matrix<double> *k = new matrix<double>(1, n);
    matrix<double>::multiply(*k, mf, invMP);

    return *k;
}

/*!
  \brief Реализует метод Эйлера.
  \param A Квадратная nxn матрица системы дифференциальных уравнений.
  \param x0 Начальное состояние - столбец nx1.
  \param h Шаг моделирования.
  \param t0 Начальное значение аргумента.
  \param t1 Конечное значение аргумента.
  \param res Вектор значений функции.
 */
void numericEuler(matrix<double> &A, matrix<double> &x0, double h,
                  double t0, double t1, vector< matrix<double> > &res)
{
    int n = x0.getRows();
    matrix<double> xk = x0,
            Axk(n, 1);

    int k = 0;
    double tk = 0;

    while (tk <= t1)
    {
        res.push_back(xk);
        tk = t0 + k * h;
        matrix<double>::multiply(Axk, A, xk);
        Axk *= h;
        xk += Axk;
        ++k;
    }
}

/*!
  \brief Создает матрицу псевдослучайных чисел от 0 до 1.
  \param n Количество строк матрицы.
  \param m Количество столбцов матрицы.
  \return Указатель на случайную матрицу.
 */
matrix<double>* randMatrix(size_t n, size_t m)
{
    matrix<double> *matr = new matrix<double>(n, m);

    srand((unsigned)(RAND_MAX * ((time(0) % 100) / 100.0)));
    for (size_t i = 0; i != n; ++i)
        for (size_t j = 0; j != m; ++j)
        {
            (*matr)(i, j) = (double)rand() / (double)RAND_MAX;
        }
    return matr;
}

}
}
