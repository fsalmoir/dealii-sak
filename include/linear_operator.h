#ifndef __dealii_linear_operator_h
#define __dealii_linear_operator_h

#include <functional>
#include <iostream>
#include <unistd.h>
#include <deal.II/base/logstream.h>

class Matrix;

template < class Vec > class LinearOperator
{
public:

  template < class InOp > LinearOperator ( const InOp &foo )
  {
    *this = foo;
  }

  LinearOperator() {};

  ~LinearOperator()
  {
    std::for_each(storage.begin(), storage.end(),  [](Vec*a)
    {
      delete a;
    });
  };

  typedef typename std::function < const void ( Vec &, const Vec &) > vmult_f;
  typedef typename std::function < Vec * ( void ) > factory_f;

  vmult_f vmult;
  factory_f new_domain_member;
  factory_f new_range_member;

  template < class InOp > LinearOperator &operator= ( const InOp &foo )
  {
    vmult =[foo] ( Vec & d, const Vec & s )
    {
      foo.vmult ( d, s );
    };

    new_domain_member = get_domain_factory(foo);
    new_range_member = get_range_factory(foo);
    return *this;
  };


  std::vector<Vec *> storage;

private:
  template<class InOp>
  factory_f get_domain_factory(const InOp &foo);

  template<class InOp>
  factory_f get_range_factory(const InOp &foo);

};

template<>
template<>
typename LinearOperator<double>::factory_f
LinearOperator<double>::get_domain_factory<Matrix>(const Matrix &foo)
{
  factory_f f = []()-> double *
  {
    return new double;
  };
  return f;
}

template<>
template<>
typename LinearOperator<double>::factory_f
LinearOperator<double>::get_range_factory<Matrix>(const Matrix &foo)
{
  factory_f f = []()-> double *
  {
    return new double;
  };
  return f;
}

template<class Vec>
template<class InOp>
typename LinearOperator<Vec>::factory_f
LinearOperator<Vec>::get_range_factory(const InOp &foo)
{
  if (typeid(InOp) == typeid(*this))
    {
      auto op = reinterpret_cast<const LinearOperator<Vec> *>(&foo);
      return op->new_range_member;
    }
  Assert(false, ExcNotImplemented());
  typename LinearOperator<Vec>::factory_f f;
  return f;
}

template<class Vec>
template<class InOp>
typename LinearOperator<Vec>::factory_f
LinearOperator<Vec>::get_domain_factory(const InOp &foo)
{
  if (typeid(InOp) == typeid(*this))
    {
      auto op = reinterpret_cast<const LinearOperator<Vec> *>(&foo);
      return op->new_range_member;
    }
  Assert(false, ExcNotImplemented());
  typename LinearOperator<Vec>::factory_f f;
  return f;
}


template<class Vec, class Scalar>
LinearOperator<Vec> *
scale(const LinearOperator<Vec> *a, const Scalar &b)
{
  auto c = new LinearOperator<Vec>();
  c->new_domain_member = a->new_domain_member;
  c->new_range_member = a->new_range_member;

  c.vmult = [a,b] ( Vec & d, const Vec & s )
  {
    a->vmult (d, s);
    d *= b;
  };
  return c;
};


template<class Vec, class Scalar>
LinearOperator<Vec> *
scale(const Scalar &b, const LinearOperator<Vec> *a)
{
  return scale(a,b);
}

template<class Vec>
LinearOperator<Vec>
operator+(const LinearOperator<Vec> &a, const LinearOperator<Vec> &b)
{
  auto c = new LinearOperator<Vec>();
  c.new_domain_member = a.new_domain_member;
  c.new_range_member = a.new_range_member;

  c.vmult = [&a,&b,&c] ( Vec & d, const Vec & s )
  {
    auto d1 = a.new_range_member();
    a.vmult ( *d1, s );
    b.vmult (d, s);
    d += *d1;
    delete d1;
  };
  return c;
};


template<class Vec>
LinearOperator<Vec> *
sum(const LinearOperator<Vec> *a, const LinearOperator<Vec> *b)
{
  auto c = new LinearOperator<Vec>;
  c->new_domain_member = a->new_domain_member;
  c->new_range_member = a->new_range_member;

  c->storage.push_back(a->new_range_member());

  c->vmult = [a,b,c]  ( Vec & d, const Vec & s )
  {
    auto d1 = c->storage[0];
    a->vmult ( *d1, s );
    b->vmult (d, s);
    d -= *d1;
  };
  return c;
};

template<class Vec>
LinearOperator<Vec> *
mult(const LinearOperator<Vec> *a, const LinearOperator<Vec> *b)
{
  auto c = new LinearOperator<Vec>;
  c->new_domain_member = b->new_domain_member;
  c->new_range_member = a->new_range_member;

  c->storage.push_back(b->new_range_member());

  c->vmult = [a,b,c] ( Vec & d, const Vec & s )
  {
    auto d1 = c->storage[0];
    b->vmult ( *d1, s );
    a->vmult (d, *d1);
  };
  return c;
};


#endif
