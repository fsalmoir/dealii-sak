//-----------------------------------------------------------
//
//    Copyright (C) 2014 by the deal.II authors
//
//    This file is subject to LGPL and may not be distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//-----------------------------------------------------------

#include "tests.h"
#include "linear_operator.h"


class Matrix
{
public:
  Matrix(double in)
  {
    k = in;
    deallog << "\t" << __PRETTY_FUNCTION__ << std::endl
            << "\t" << __FUNCTION__ << "():" << k << std::endl;

  };

  void vmult ( double &d, const double &s ) const
  {
    deallog << "\t" << __PRETTY_FUNCTION__ << std::endl
            << "\t" << __FUNCTION__ << "():"
            << " (K*s) == (" << k
            << " * " << s << ")"
            << std::endl;
    d = k * s;
  };

private:
  double k = 3.;
};

int main ()
{
  initlog();
  deallog << 0 << std::endl;

  Matrix A(2.);
  Matrix B(5.);

  LinearOperator < double >OpA(A);
  LinearOperator < double >OpB = B;
  LinearOperator < double >OpResult;
  LinearOperator < double >OpResult2;

  double dst = -1;
  double src = 3.;

# define TEST(res) {                 \
    deallog << ">>>>>>>> testing [" << #res" ] input=" << src << std::endl;  \
    res->vmult(dst,src);                 \
    deallog << "[ result=" << dst << " <<<<<<<<" << std::endl; \
    deallog << std::endl;                 \
    delete res;         \
  }

  auto OpRes = sum(&OpA, &OpB);
  auto OpRes1 = mult(&OpA, &OpB);

  TEST(OpRes);
  TEST(OpRes1);

  return 0;
}
