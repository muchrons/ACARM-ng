/*
 * EnsurePtrNotNULL.t.cpp
 *
 */
#include <tut.h>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/equal.hpp>

#include "Algo/MPL/EnsurePtrNotNULL.hpp"

using namespace boost;
using namespace Commons;
using namespace Algo::MPL;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/MPL/EnsurePtrNotNULL");
} // unnamed namespace


namespace tut
{

// check type for shared_ptr<>
template<>
template<>
void testObj::test<1>(void)
{
  typedef shared_ptr<double>             Tested;
  typedef EnsurePtrNotNULL<Tested>::type Out;
  typedef SharedPtrNotNULL<double>       Expected;
  ensure("invalid type for shared_ptr<double>", is_same<Out, Expected>::type::value);
}

// test shared_ptr with const
template<>
template<>
void testObj::test<2>(void)
{
  typedef shared_ptr<const double>       Tested;
  typedef EnsurePtrNotNULL<Tested>::type    Out;
  typedef SharedPtrNotNULL<const double> Expected;
  ensure("invalid type for shared_ptr<const double>", is_same<Out, Expected>::type::value);
}

// test SharedPtrNotNULL<>
template<>
template<>
void testObj::test<3>(void)
{
  typedef SharedPtrNotNULL<double>    Tested;
  typedef EnsurePtrNotNULL<Tested>::type Out;
  typedef SharedPtrNotNULL<double>    Expected;
  ensure("invalid type for SharedPtrNotNULL<double>", is_same<Out, Expected>::type::value);
}

// test SharedPtrNotNULL<> with const
template<>
template<>
void testObj::test<4>(void)
{
  typedef SharedPtrNotNULL<const double> Tested;
  typedef EnsurePtrNotNULL<Tested>::type    Out;
  typedef SharedPtrNotNULL<const double> Expected;
  ensure("invalid type for SharedPtrNotNULL<const double>", is_same<Out, Expected>::type::value);
}

} // namespace tut
