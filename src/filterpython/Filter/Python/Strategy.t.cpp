/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/Python/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types::Proc;
using namespace Filter::Python;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void):
    n_("somename"),
    s_(n_, Config(600, "testdata/long_names_compare.py"))
  {
  }

  GraphNodePtrNN mkLeaf(const char *name) const
  {
    return makeNewLeaf( makeNewAlert(name) );
  }

  Strategy::ChangedNodes changed_;
  const InstanceName     n_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Python/Strategy");
} // unnamed namespace

namespace tut
{

// test error when sript does not declare dervied object
template<>
template<>
void testObj::test<1>(void)
{
  const Config c(11, "testdata/null_ptr.py");
  try
  {
    Strategy s(n_, c);
    fail("no exception thrown when unable to build derived class");
  }
  catch(const Commons::ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test correlation of two alerts
template<>
template<>
void testObj::test<2>(void)
{
  s_.process(mkLeaf("some alert"), changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(mkLeaf("some alert"), changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
}

// test if can correltate will return false for defferent names
template<>
template<>
void testObj::test<3>(void)
{
  s_.process(mkLeaf("some alert"), changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0u);

  s_.process(mkLeaf("OTHER alert"), changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0u);
}

// test getting name of meta alert
template<>
template<>
void testObj::test<4>(void)
{
  s_.process(mkLeaf("some alert"), changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(mkLeaf("some alert"), changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
  const string resp("[python] /script/ some alert");
  ensure_equals("invalid name", changed_[0]->getMetaAlert()->getName().get(), resp);
}

// test correlation of three alerts
template<>
template<>
void testObj::test<5>(void)
{
  s_.process(mkLeaf("some alert"), changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(mkLeaf("some alert"), changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
  ConstGraphNodePtrNN correlated=changed_.at(0);
  changed_.clear();

  s_.process(mkLeaf("some alert"), changed_);
  ensure_equals("no nodes changed - 3rd alerts' correaltion failed", changed_.size(), 1u);
  ensure("correlation not added to existing node", correlated.get()==changed_.at(0).get() );
}

// test exception when python's c-tor throws
template<>
template<>
void testObj::test<6>(void)
{
  const Config c(11, "testdata/throw_in_ctor.py");
  try
  {
    Strategy s(n_, c);
    fail("exception not re-thrown from python");
  }
  catch(const PythonAPI::Exception&)
  {
    // this is expected
  }
}

// test exception when script does not exist
template<>
template<>
void testObj::test<7>(void)
{
  const Config c(11, "testdata/script_that_does_not_exist.py");
  try
  {
    Strategy s(n_, c);
    fail("no exception thrown when file does not exist");
  }
  catch(const Commons::Exception&)
  {
    // this is expected
  }
}

// test if exception is NOT propagated when error is risen in python script's methods
template<>
template<>
void testObj::test<8>(void)
{
  const Config c(11, "testdata/throw_in_method.py");
  Strategy     s(n_, c);
  s.process(mkLeaf("whatever1"), changed_); // must not throw
  s.process(mkLeaf("whatever2"), changed_); // must not throw
}

// test setting data in python
template<>
template<>
void testObj::test<9>(void)
{
  const Config c(11, "testdata/data_checker.py");
  Strategy     s(n_, c);
  s.process(mkLeaf("whatever1"), changed_); // must not throw
}

// test reading previously set data (in python)
template<>
template<>
void testObj::test<10>(void)
{
  const Config c(11, "testdata/data_checker.py");
  Strategy     s(n_, c);
  s.process(mkLeaf("whatever1"), changed_); // must not throw
  s.process(mkLeaf("whatever1"), changed_); // must not throw
}

} // namespace tut
