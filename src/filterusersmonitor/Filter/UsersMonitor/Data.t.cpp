/*
 * Data.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <algorithm>
#include <cassert>

#include "Filter/UsersMonitor/Data.hpp"
#include "Filter/UsersMonitor/TestBase.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::UsersMonitor;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestBase
{
  Data::Names skip_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/UsersMonitor/Data");
} // unnamed namespace


namespace tut
{

// test if creating from alert with processes w/o users is NULL.
template<>
template<>
void testObj::test<1>(void)
{
  const Data d( mkAlert(NULL, NULL, NULL, NULL, true), skip_ );
  ensure_equals("processes w/o users generated some output", d.get().size(), 0u);
}

// test if creating from alert w/o processes is NULL.
template<>
template<>
void testObj::test<2>(void)
{
  const Data d( mkAlert(NULL, NULL, NULL, NULL, false), skip_ );
  ensure_equals("alert w/o processes generated some output", d.get().size(), 0u);
}

// test if collected values are fine
template<>
template<>
void testObj::test<3>(void)
{
  const Data  d( mkAlert("alice", "cat", "tom", "jerry"), skip_ );
  Data::Names out=d.get();
  sort( out.begin(), out.end() );   // for sake of test's simplicity
  ensure_equals("invalid number of elements", out.size(), 4u);
  ensure_equals("invalid element 1", out[0], "alice");
  ensure_equals("invalid element 2", out[1], "cat"  );
  ensure_equals("invalid element 3", out[2], "jerry");
  ensure_equals("invalid element 4", out[3], "tom"  );
}

// test swap
template<>
template<>
void testObj::test<4>(void)
{
  Data d1( mkAlert("alice"), skip_ );
  Data d2( mkAlert("cat"), skip_   );
  d1.swap(d2);
  // check
  ensure_equals("invalid size 1", d1.get().size(), 1u);
  ensure_equals("invalid size 2", d2.get().size(), 1u);
  ensure_equals("invalid value 1", d1.get()[0], "cat"  );
  ensure_equals("invalid value 2", d2.get()[0], "alice");
}

// test intersection when there is none
template<>
template<>
void testObj::test<5>(void)
{
  const Data d1( mkAlert("alice"), skip_ );
  const Data d2( mkAlert("cat"), skip_   );
  Data::Names::const_iterator it=d1.commonWith(d2);
  ensure("something has been found", it==d1.get().end() );
}

// test intersection when there is common element
template<>
template<>
void testObj::test<6>(void)
{
  const Data d1( mkAlert("alice", "dog"), skip_ );
  const Data d2( mkAlert("dog", "yeti"), skip_   );
  Data::Names::const_iterator it=d1.commonWith(d2);
  ensure("nothing has been returned", it!=d1.get().end() );
  ensure_equals("invalid element returned", *it, "dog");
}

// test creating data from single entry
template<>
template<>
void testObj::test<7>(void)
{
  const Data d("abc", skip_);
  ensure_equals("invalid elements count", d.get().size(), 1u);
  ensure_equals("invalid element returned", d.get()[0], "abc");
}

// test creating empty set
template<>
template<>
void testObj::test<8>(void)
{
  Data d;
  ensure_equals("invalid elements count", d.get().size(), 0u);
}

// test interesection of the name that is no the skip list
template<>
template<>
void testObj::test<9>(void)
{
  skip_.push_back("alice");
  const Data d1( mkAlert("alice"), skip_ );
  const Data d2( mkAlert("alice"), skip_ );
  Data::Names::const_iterator it=d1.commonWith(d2);
  ensure("something has been found", it==d1.get().end() );
}

// test if skip list is checked on 1-arg c-tor as well.
template<>
template<>
void testObj::test<10>(void)
{
  skip_.push_back("abc");
  const Data d("abc", skip_);
  ensure_equals("skip-list not applied", d.get().size(), 0u);
}

// check if duplicated names are remvoed
template<>
template<>
void testObj::test<11>(void)
{
  const Data d( mkAlert("alice", "cat", "doom", "alice"), skip_ );
  Data::Names out=d.get();
  sort( out.begin(), out.end() );   // for sake of test's simplicity
  ensure_equals("invalid number of elements", out.size(), 3u);
  ensure_equals("invalid element 1", out[0], "alice");
  ensure_equals("invalid element 2", out[1], "cat"  );
  ensure_equals("invalid element 3", out[2], "doom" );
}

// check if "" name is rejected
template<>
template<>
void testObj::test<12>(void)
{
  const Data  d( mkAlert("abc", "", NULL, NULL), skip_ );
  Data::Names out=d.get();
  sort( out.begin(), out.end() );   // for sake of test's simplicity
  ensure_equals("invalid number of elements", out.size(), 1u);
  ensure_equals("invalid element", out[0], "abc");
}

} // namespace tut
