/*
 * Hash.t.cpp
 *
 */

#include <tut.h>

#include "Filter/NewEvent/Hash.hpp"

using namespace Filter::NewEvent;
using namespace std;

namespace
{

struct TestClass
{
  TestClass(void):
    h1_("key"),
    h2_("other key")
  {
  }

  Hash h1_;
  Hash h2_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/Hash");
} // unnamed namespace

namespace tut
{

// test c-tor, smoke test
template<>
template<>
void testObj::test<1>(void)
{
  Hash hash("key");
}

// test getHash() method
template<>
template<>
void testObj::test<2>(void)
{
  const string sha1("a62f2225bf70bfaccbc7f1ef2a397836717377de");
  ensure_equals("invalid hash", h1_.getHash().get(), sha1);

}
// test '==' operator
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("", h1_ == h2_, false);
}

// test '!=' operator
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("", h1_ != h2_, true);
}

} // namespace tut
