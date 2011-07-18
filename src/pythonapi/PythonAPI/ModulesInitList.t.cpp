/*
 * ModulesInitList.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "PythonAPI/ModulesInitList.hpp"

using namespace std;
using namespace PythonAPI;

extern "C"
{
static void testInitCall(void)
{
  // nothing has to be done
} // testInitCall()
} // extern "C"

namespace
{
struct TestClass
{
  ModulesInitList m_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/ModulesInitList");
} // unnamed namespace

namespace tut
{

// test initial size
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid initial size", m_.count(), 0);
}

// test initiali iterators
template<>
template<>
void testObj::test<2>(void)
{
  ensure("iterators indicate non-empty collection", m_.begin()==m_.end() );
}

// try adding some element
template<>
template<>
void testObj::test<3>(void)
{
  m_.scheduleImport("test", testInitCall);
  ensure_equals("invalid size", m_.count(), 1);
  ModulesInitList::const_iterator it=m_.begin();
  ensure_equals("invalid name", it->get<0>(), string("test") );
  ensure_equals("invalid init call", it->get<1>(), testInitCall);
  ++it;
  ensure("more than one element in collection", it==m_.end() );
}

// test erasure of the collection's content
template<>
template<>
void testObj::test<4>(void)
{
  m_.scheduleImport("test1", testInitCall);
  m_.scheduleImport("test2", testInitCall);
  ensure_equals("invalid size after additions", m_.count(), 2);
  m_.clear();
  ensure_equals("invalid size after clearing", m_.count(), 0);
}

} // namespace tut
