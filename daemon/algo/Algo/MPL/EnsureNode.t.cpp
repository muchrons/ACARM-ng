/*
 * EnsureNode.t.cpp
 *
 */
#include <tut.h>
#include <boost/syhared_ptr.hpp>
#include <boost/mpl/equal.hpp>

#include "Algo/MPL/EnsureNode.hpp"

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

factory tf("Algo/MPL/EnsureNode");
} // unnamed namespace


namespace tut
{

// check type for shared_ptr<GraphNode>
template<>
template<>
void testObj::test<1>(void)
{
  typedef shared_ptr<GraphNode>       Tested;
  typedef EnsureNode<Tested>::type    Out;
  typedef SharedPtrNotNULL<GraphNode> Expected;
  ensure("invalid type for shared_ptr<GraphNode>", mpl::equal<Out, Expected>::type::value);
}

// test shared_ptr with const
template<>
template<>
void testObj::test<2>(void)
{
  typedef shared_ptr<const GraphNode>       Tested;
  typedef EnsureNode<Tested>::type          Out;
  typedef SharedPtrNotNULL<const GraphNode> Expected;
  ensure("invalid type for shared_ptr<const GraphNode>", mpl::equal<Out, Expected>::type::value);
}

// test SharedPtrNotNULL<>
template<>
template<>
void testObj::test<3>(void)
{
  typedef SharedPtrNotNULL<GraphNode> Tested;
  typedef EnsureNode<Tested>::type    Out;
  typedef SharedPtrNotNULL<GraphNode> Expected;
  ensure("invalid type for SharedPtrNotNULL<GraphNode>", mpl::equal<Out, Expected>::type::value);
}

// test SharedPtrNotNULL<> with const
template<>
template<>
void testObj::test<4>(void)
{
  typedef SharedPtrNotNULL<const GraphNode> Tested;
  typedef EnsureNode<Tested>::type          Out;
  typedef SharedPtrNotNULL<const GraphNode> Expected;
  ensure("invalid type for SharedPtrNotNULL<const GraphNode>", mpl::equal<Out, Expected>::type::value);

  // TODO: these should NOT compile
  EnsureNode< std::auto_ptr<GraphNode> >::type;
  EnsureNode< shared_ptr<double> >::type;
}

} // namespace tut
