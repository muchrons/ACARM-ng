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
using namespace Persistency;
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

// check type for shared_ptr<GraphNode>
template<>
template<>
void testObj::test<1>(void)
{
  typedef shared_ptr<GraphNode>          Tested;
  typedef EnsurePtrNotNULL<Tested>::type Out;
  typedef SharedPtrNotNULL<GraphNode>    Expected;
  ensure("invalid type for shared_ptr<GraphNode>", is_same<Out, Expected>::type::value);
}

// test shared_ptr with const
template<>
template<>
void testObj::test<2>(void)
{
  typedef shared_ptr<const GraphNode>       Tested;
  typedef EnsurePtrNotNULL<Tested>::type    Out;
  typedef SharedPtrNotNULL<const GraphNode> Expected;
  ensure("invalid type for shared_ptr<const GraphNode>", is_same<Out, Expected>::type::value);
}

// test SharedPtrNotNULL<>
template<>
template<>
void testObj::test<3>(void)
{
  typedef SharedPtrNotNULL<GraphNode>    Tested;
  typedef EnsurePtrNotNULL<Tested>::type Out;
  typedef SharedPtrNotNULL<GraphNode>    Expected;
  ensure("invalid type for SharedPtrNotNULL<GraphNode>", is_same<Out, Expected>::type::value);
}

// test SharedPtrNotNULL<> with const
template<>
template<>
void testObj::test<4>(void)
{
  typedef SharedPtrNotNULL<const GraphNode> Tested;
  typedef EnsurePtrNotNULL<Tested>::type    Out;
  typedef SharedPtrNotNULL<const GraphNode> Expected;
  ensure("invalid type for SharedPtrNotNULL<const GraphNode>", is_same<Out, Expected>::type::value);
}

// this test should NOT compile
template<>
template<>
void testObj::test<5>(void)
{
  //EnsurePtrNotNULL< shared_ptr<double> >::type b;   // error: invalid type in pointer
}

} // namespace tut
