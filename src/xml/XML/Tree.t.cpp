/*
 * Tree.t.cpp
 *
 */
#include <tut.h>

#include "XML/Tree.hpp"


namespace XML
{

struct TreeTestClass
{
};

} // namespace XML


namespace tut
{
typedef XML::TreeTestClass TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("XML/Tree");
}

using namespace std;
using namespace XML;


namespace tut
{

// test if there is non-const getter.
template<>
template<>
void testObj::test<1>(void)
{
  Tree  t( Node("abc") );
  Node &n=t.getRoot();      // compilation-time test
  n.getName();              // suppresses compiler warring
}

// test if there is const getter.
template<>
template<>
void testObj::test<2>(void)
{
  const Tree  t( Node("xyz") );
  const Node &n=t.getRoot();// compile-time test
  n.getName();              // suppresses compiler warring
}

// test shared_ptr-like copyablility semantics.
template<>
template<>
void testObj::test<3>(void)
{
  const Tree t1( Node("xyz") );
  const Tree t2=t1;
  ensure("internal copy has been made", &t1.getRoot()==&t2.getRoot() );
}

} // namespace tut
