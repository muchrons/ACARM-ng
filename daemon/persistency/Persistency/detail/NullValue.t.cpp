/*
 * NullValue.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/detail/NullValue.hpp"

using namespace std;
using namespace Persistency::detail;

namespace
{

struct NullValueTestClass
{
};

typedef NullValueTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/detail/NullValue");
} // unnamed namespace


namespace tut
{

// test passing some pointer
template<>
template<>
void testObj::test<1>(void)
{
  const double x=3.14;
  const NullValue<double> nv(&x);
  ensure("NULL pointer recieved", nv.get()!=NULL);
  ensure("invalid pointer",       nv.get()!=&x);
  ensure_equals("invalid value", *nv.get(), x);
}

// test passing NULL
template<>
template<>
void testObj::test<2>(void)
{
  const NullValue<double> nv(NULL);
  ensure("non-NULL pointer recieved", nv.get()==NULL);
}

// test default c-tor
template<>
template<>
void testObj::test<3>(void)
{
  NullValue<double> nv;
  ensure("non-NULL pointer recieved", nv.get()==NULL);
}

} // namespace tut
