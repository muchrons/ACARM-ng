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
    hash_("key")
  {
  }

  Hash hash_;
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
  ensure_equals("invalid hash", string(hash_.getHash().get()), sha1);

}
// test '==' and '!=' operators
template<>
template<>
void testObj::test<3>(void)
{
  Hash h("other key");
  ensure_equals("", h == hash_, false);
  ensure_equals("", h != hash_, true);
}

} // namespace tut
