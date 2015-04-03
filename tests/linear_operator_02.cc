#include <functional>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cmath>

class Matrix
{
public:
  Matrix(double in)
  {
    k = in;
  };

  void vmult ( double &d, const double &s ) const
  {
    std::cout << "\t\t" << __PRETTY_FUNCTION__ << std::endl;
    std::cout
        << "\t\t" << __FUNCTION__ << "():"
        << " (s * k) == (" << s
        << " * " << k << ")"
        << std::endl;
    d = k * s;
  };
  double get_k(void)
  {
    return k ;
  };
  void set_k(double in)
  {
    k = in;
  };
  void operator= ( const Matrix &foo )
  {
    k = foo.k;
  };

private:
  double k = 3.;
};

template < class Vec > class LinearOperator
{
public:

  template < class InOp > LinearOperator ( const InOp &foo )
  {
    *this = foo;
    this->funcptr_stack.push_back(this->vmult);
//    this->funcptr_stack = foo.funcptr_stack;
  }

  LinearOperator() {};

  std::function < const void ( Vec &, const Vec &) > vmult;
  std::function < Vec * ( void ) > new_domain_member;
  std::function < Vec * ( void ) > new_range_member;

  template < class InOp > LinearOperator &operator= ( const InOp &foo )
  {
    vmult =[foo] ( Vec & d, const Vec & s )
    {
      foo.vmult ( d, s );
    };
    if ( auto matrix = dynamic_cast < const Matrix * >( &foo ) )
      {
        new_domain_member = [matrix]()->Vec *
        {
          return new Vec;
        };
        new_range_member = [matrix]()->Vec *
        {
          return new Vec;
        };
      }
    return *this;
  };

  template < class InOp > LinearOperator &operator+= ( const InOp &foo )
  {
    *this + foo;
    return *this;
  };

  template < class InOp > LinearOperator &operator-= ( const InOp &foo )
  {
    *this - foo;
    return *this;
  };

  template < class InOp > LinearOperator &operator* ( const InOp &foo )
  {
    if ( auto matrix = reinterpret_cast < const Matrix * >( &foo ) )
      {
        new_domain_member = [matrix]()->Vec *
        {
          return new Vec;
        };
        new_range_member = [this]()->Vec *
        {
          return new Vec;
        };
      }
    if ( auto op = reinterpret_cast < const LinearOperator< Vec > * >( &foo ) )
      {
        new_domain_member = op->new_domain_member;
        new_range_member = op->new_range_member;
      }

    std::cout << __FUNCTION__ << "()[" << __LINE__ << "]: PRE  funcptr_stack=" << &funcptr_stack[funcptr_stack.size()-1] << std::endl;
    funcptr_stack.push_back(vmult);
    std::cout << __FUNCTION__ << "()[" << __LINE__ << "]: POST funcptr_stack=" << &funcptr_stack[funcptr_stack.size()-1] << std::endl;
    int index = funcptr_stack.size()-1;

    vmult =[&foo,this,index] ( Vec & d, const Vec & s )
    {
      std::cout
          << "\t" << __FUNCTION__ << "(lambda)[" << __LINE__ << "]: d=" << d
          << " s=" << s << " funcptr[" << index << "]=" << &this->funcptr_stack[index]
          << std::endl;
      usleep(100000);
      Vec *d1 = new_range_member();
      foo.vmult( *d1, s );
      this->funcptr_stack[index]( d, *d1 );
      delete d1;
    };
    return *this;
  };

  template < class InOp > LinearOperator &operator+ ( const InOp &foo )
  {
    if ( auto matrix = reinterpret_cast < const Matrix * >( &foo ) )
      {
        new_domain_member = [matrix]()->Vec *
        {
          return new Vec;
        };
        new_range_member = [this]()->Vec *
        {
          return new Vec;
        };
      }
    if ( auto op = reinterpret_cast < const LinearOperator< Vec > * >( &foo ) )
      {
        new_domain_member = op->new_domain_member;
        new_range_member = op->new_range_member;
      }

    std::cout << __FUNCTION__ << "()[" << __LINE__ << "]: PRE  funcptr_stack=" << &funcptr_stack[funcptr_stack.size()-1] << std::endl;
    funcptr_stack.push_back(vmult);
    std::cout << __FUNCTION__ << "()[" << __LINE__ << "]: POST funcptr_stack=" << &funcptr_stack[funcptr_stack.size()-1] << std::endl;
    int index = funcptr_stack.size()-1;

    vmult =[foo,this,index] ( Vec & d, const Vec & s )
    {
      std::cout
          << "\t" << __FUNCTION__ << "(lambda)[" << __LINE__ << "]: d=" << d
          << " s=" << s << " funcptr[" << index << "]=" << &this->funcptr_stack[index]
          << std::endl;
      usleep(100000);
      Vec *d1 = new_range_member();
      foo.vmult( *d1, s );
      this->funcptr_stack[index]( d, s );
      d += *d1;
      delete d1;
    };
    return *this;
  };

  template < class InOp > LinearOperator &operator- ( const InOp &foo )
  {
    if ( auto matrix = reinterpret_cast < const Matrix * >( &foo ) )
      {
        new_domain_member = [matrix]()->Vec *
        {
          return new Vec;
        };
        new_range_member = [this]()->Vec *
        {
          return new Vec;
        };
      }
    if ( auto op = reinterpret_cast < const LinearOperator< Vec > * >( &foo ) )
      {
        new_domain_member = op->new_domain_member;
        new_range_member = op->new_range_member;
      }

    std::cout << __FUNCTION__ << "()[" << __LINE__ << "]: PRE  funcptr_stack=" << &funcptr_stack[funcptr_stack.size()-1] << std::endl;
    funcptr_stack.push_back(vmult);
    std::cout << __FUNCTION__ << "()[" << __LINE__ << "]: POST funcptr_stack=" << &funcptr_stack[funcptr_stack.size()-1] << std::endl;
    int index = funcptr_stack.size()-1;

    vmult =[foo,this,index] ( Vec & d, const Vec & s )
    {
      std::cout
          << "\t" << __FUNCTION__ << "(lambda)[" << __LINE__ << "]: d=" << d
          << " s=" << s << " funcptr[" << index << "]=" << &this->funcptr_stack[index]
          << std::endl;
      usleep(100000);
      Vec *d1 = new_range_member();
      foo.vmult( *d1, s );
      this->funcptr_stack[index]( d, s );
      d -= *d1;
      delete d1;
    };
    return *this;
  };

//  void clear(void) { funcptr_stack.clear(); };
  void clear(void)
  {
    funcptr_stack.erase(funcptr_stack.begin()-1);
  };

private:
  std::function < const void ( Vec &, const Vec &) > orig_vmult;
  std::vector < std::function<const void (Vec &,const Vec &)> > funcptr_stack;

};


int main ()
{
  Matrix A(2.);
  Matrix B(7.);
  Matrix C(10.);

  A.set_k(5.);

  LinearOperator < double >OpA(A);
  LinearOperator < double >OpB = B;
  LinearOperator < double >OpResult;
  LinearOperator < double >OpC(C);

  double dst = -1;
  double src = 3.;

#if 1
# undef TEST
# define TEST(res,op,exp) {               \
    double result = exp;                  \
    std::cout << ">>>>>>>> testing [" << #res" = "#op << "] input=" << src << std::endl;  \
    res = op;                   \
    res.vmult(dst,src);                 \
    std::cout << "[" << #res" = "#op << "] result=" << dst << " <<<<<<<<" << std::endl; \
    std::cout << "expected result: " << #exp << " = " << result << std::endl;   \
    if ( fabs(result - dst) > 1e-9 )              \
      {                     \
        std::cout                 \
            << "################## BAD: dst != res,"      \
            << " result=" << dst << " expected=" << result << std::endl ; \
        sleep(1);                 \
      }                     \
    std::cout << std::endl;                 \
  }
#else
# undef TEST
# define TEST(a...) {}
#endif

#define BAD

#if defined BAD
# define CLEAR {OpResult.clear(); OpA.clear(); OpB.clear();}
#elif defined WORKISH
# define CLEAR {OpA=A;}
#else
# define CLEAR {}
#endif

  double Ak= A.get_k();
  double Bk= B.get_k();
  double Ck= C.get_k();

  std::cout << "A.k = " << Ak << std::endl;
  std::cout << "B.k = " << Bk << std::endl;
  std::cout << "C.k = " << Ck << std::endl;

  // TEST(OpResult, OpA             , (Ak*src)               ); CLEAR;
  // TEST(OpResult, OpB             , (Bk*src)               ); CLEAR;

  // TEST(OpResult, OpA * OpB       , (Ak*src)*(Bk)          ); CLEAR;
  // TEST(OpResult, OpA + OpB       , (Ak*src)+(Bk*src)      ); CLEAR;
  // TEST(OpA,      OpA + OpB       , (Ak*src)+(Bk*src)      ); CLEAR;
  // TEST(OpResult, OpA * OpB       , (Ak*src)*(Bk)          ); CLEAR;
  // TEST(OpResult, OpA - OpB       , (Ak*src)-(Bk*src)      ); CLEAR;
  // TEST(OpResult, OpA * OpB - OpB , (Ak*src)*(Bk)-(Bk*src) ); CLEAR;
  // TEST(OpResult, OpA - OpB + OpB , (Ak*src)               ); CLEAR;
  // TEST(OpResult, OpA + OpA       , (Ak*src)+(Ak*src)      ); CLEAR;
  // TEST(OpResult, OpA - OpA       , (0)                    ); CLEAR;
  // TEST(OpResult, OpA * OpB * OpB , (Ak*src)*(Bk)*(Bk)     ); CLEAR;

  // // bad
  // C = A; Ck = C.get_k();
  // std::cout << "A.k = " << Ak << std::endl;
  // std::cout << "C.k = " << Ck << std::endl;
  // TEST(OpResult, OpA - OpA + OpA , (Ak*src)-(Ak*src)+(Ak*src) ); CLEAR;
  // TEST(OpResult, OpA - OpC + OpA , (Ak*src)-(Ck*src)+(Ak*src) ); CLEAR;
  // TEST(OpResult, OpA - OpA + OpC , (Ak*src)-(Ak*src)+(Ck*src) ); CLEAR;
  // TEST(OpResult, OpA - OpB + OpC , (Ak*src)-(Bk*src)+(Ck*src) ); CLEAR;

  // // very bad, loops forever
  // std::cerr << "*** the next test will loop forever, use CTRL-C" << std::endl << std::endl;
  // sleep(1);
  // TEST(OpResult, OpA * OpB * OpA , (Ak*src)*(Bk)*(Ak) ); CLEAR;

  return 0;
}
