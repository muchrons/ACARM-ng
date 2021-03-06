/*
 * NullValue.t.cpp
 *
 */
#include <tut.h>

#include "Base/NullValue.hpp"

using namespace std;
using namespace Base;

namespace
{

struct TestClass
{
  // NOTE: this code is identical to the one in TestHelpers/checkEquality.hpp
  //       but it cannot be included, since it would create dependency cycle. :/
  template<typename T>
  void checkEquality(const T &t1, const T &t1o, const T &t2)
  {
    // test equality
    tut::ensure("comparing identical elements with == failed",          t1==t1o  );
    // test if equality fails for different eleemnts
    tut::ensure("comparing different elements with == didn't failed", !(t1==t2)  );

    // test inequality
    tut::ensure("comparing identical elements with != didn't failed", !(t1!=t1o) );
    // test if equality fails for different eleemnts
    tut::ensure("comparing different elements with != failed",          t1!=t2   );
  }

  template<typename T>
  void checkEquality(const T &t1, const T &t2)
  {
    checkEquality(t1, t1, t2);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/NullValue");
} // unnamed namespace


namespace tut
{

// test passing some pointer
template<>
template<>
void testObj::test<1>(void)
{
  const double            x=3.14;
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
  const double            *null=NULL;
  const NullValue<double>  nv(null);
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

// test comparing NULLs
template<>
template<>
void testObj::test<4>(void)
{
  NullValue<double> nv1;
  NullValue<double> nv2;
  ensure("NULLs are not the same", nv1==nv2);
}

// test creating from values instead of pointer
template<>
template<>
void testObj::test<5>(void)
{
  NullValue<double> nv(4.2);
  ensure("got NULL from value",  nv.get()!=NULL);
  ensure("got invalid value",   *nv.get()==4.2 );
}

// test comparing values.
template<>
template<>
void testObj::test<6>(void)
{
  NullValue<double> nv1 (1.2);
  NullValue<double> nv1o(1.2);
  NullValue<double> nv2 (1.4);
  checkEquality(nv1, nv1o, nv2);
}

// test comparing values with NULLs.
template<>
template<>
void testObj::test<7>(void)
{
  NullValue<double> nv1(1.2);
  NullValue<double> nv2;
  checkEquality(nv1, nv2);
}

// test for warning when no value is set.
template<>
template<>
void testObj::test<8>(void)
{
  NullValue<int> nv2;
  ensure("value's not NULL for default c-tor", nv2.get()==NULL );
}

// test pointer-like access (arrow operator)
template<>
template<>
void testObj::test<9>(void)
{
  const std::string      str("hello world");
  NullValue<std::string> nv(str);
  ensure_equals("invalid value", nv->c_str(), str);
}

} // namespace tut
