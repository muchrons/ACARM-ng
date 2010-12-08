/*
 * NonEmptyVector.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Base/NonEmptyVector.hpp"

using namespace std;
using namespace Base;

namespace
{
struct TestClass
{
  typedef std::string           Type;
  typedef NonEmptyVector<Type>  NEV;

  TestClass(void):
    e1_("e1"),
    e2_("e2")
  {
  }

  Type e1_;
  Type e2_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/NonEmptyVector");
} // unnamed namespace


namespace tut
{

// test default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const NEV c(e1_);
  ensure_equals("invalid size", c.size(), 1u);
}

// test const iterators
template<>
template<>
void testObj::test<2>(void)
{
  const NEV           c(e1_);
  NEV::const_iterator it=c.begin();
  ensure("empty collection", it!=c.end() );
  ensure_equals("invlaid first element", *it, e1_);
  ++it;
  ensure("too many elements in collection", it==c.end() );
}

// test iterators
template<>
template<>
void testObj::test<3>(void)
{
  NEV           c(e1_);
  NEV::iterator it=c.begin();
  ensure("empty collection", it!=c.end() );
  ensure_equals("invlaid first element", *it, e1_);
  ++it;
  ensure("too many elements in collection", it==c.end() );
}

// test adding next element
template<>
template<>
void testObj::test<4>(void)
{
  NEV c(e1_);
  c.push_back(e2_);

  NEV::iterator it=c.begin();

  ensure("empty collection", it!=c.end() );
  ensure_equals("invlaid first element", *it, e1_);
  ++it;

  ensure("just one element in collection", it!=c.end() );
  ensure_equals("invlaid second element", *it, e2_);
  ++it;

  ensure("too many elements in collection", it==c.end() );
}

// test copying
template<>
template<>
void testObj::test<5>(void)
{
  const NEV c1(e1_);
  NEV       c2(e2_);
  ensure_equals("empty collection 1", c1.size(), 1u);
  ensure_equals("empty collection 2", c2.size(), 1u);
  ensure("pre-condition failed", *c1.begin()!=*c2.begin() );

  c2=c1;
  ensure_equals("empty collection 1/after", c1.size(), 1u);
  ensure_equals("empty collection 2/after", c2.size(), 1u);
  ensure("copying failed", *c1.begin()==*c2.begin() );
}

// test random-access operator
template<>
template<>
void testObj::test<6>(void)
{
  const NEV c(e1_);
  ensure_equals("empty collection", c.size(), 1u);
  ensure_equals("invalid element", c[0], e1_);
}

} // namespace tut
