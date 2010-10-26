/*
 * Dumper.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <sstream>
#include <boost/filesystem.hpp>

#include "Commons/Convert.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "PDump/Dumper.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace PDump;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{
struct TestClass: public TestStubs
{
  TestClass(void):
    d_(out_, errOut_)
  {
  }

  ~TestClass(void)
  {
    // clean-up files created during test
    for(Dumper::NodesVector::const_iterator it=nodes_.begin(); it!=nodes_.end(); ++it)
      try
      {
        const MetaAlert::ID::Numeric id  =(*it)->getMetaAlert().getID().get();
        const path                   file="idmef_" + Commons::Convert::to<string>(id) + ".xml";
        remove(file);
      }
      catch(...)
      {
        // nothing can be done...
      }
  }

  std::stringstream   out_;
  std::stringstream   errOut_;
  Dumper              d_;
  Dumper::NodesVector nodes_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("PDump/Dumper");
} // unnamed namespace


namespace tut
{

// test dumping no alerts
template<>
template<>
void testObj::test<1>(void)
{
  d_.restoreBetween( Timestamp(1), Timestamp(1287126857), nodes_ );
  ensure_equals("some elements have been read", nodes_.size(), 0u);
}

// test from>to case
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    d_.restoreBetween( Timestamp(1287126857), Timestamp(1), nodes_ );
    fail("restoring didn't throw on invalid range");
  }
  catch(const Persistency::IO::Restorer::ExceptionInvalidTimeSpan &)
  {
    // this is expected
  }
}

// test writing nodes to non-exisitng directory
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    d_.writeToDir(nodes_, "/i/do/not/exist");
    fail("writing didn't failed for non-existing directory");
  }
  catch(const std::runtime_error &)
  {
    // this is expected
  }
}

// test writing to non-sane directory
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    d_.writeToDir(nodes_, "testdata/dirSymlink");
    fail("writing didn't failed for non-sane directory");
  }
  catch(const std::runtime_error &)
  {
    // this is expected
  }
}

// test writing some leafs
template<>
template<>
void testObj::test<5>(void)
{
  nodes_.push_back( makeNewLeaf( makeNewAlert(), 123u ) );
  const Dumper::Stats out=d_.writeToDir(nodes_, ".");
  ensure_equals("invalid number of elements", out.getTotal(), 1u);
  ensure_equals("invalid number of attempts", out.getAttempts(), 1u);
  ensure_equals("invalid number of writes", out.getWrites(), 1u);
  ensure("alert not written", Commons::Filesystem::isFileSane("idmef_123.xml") );
}

// test writing some nodes
template<>
template<>
void testObj::test<6>(void)
{
  GraphNodePtrNN leaf1=makeNewLeaf( makeNewAlert(), 123u );
  GraphNodePtrNN leaf2=makeNewLeaf( makeNewAlert(), 124u );
  nodes_.push_back( makeNewNode(leaf1, leaf2, 200u) );
  const Dumper::Stats out=d_.writeToDir(nodes_, ".");
  ensure_equals("invalid number of elements", out.getTotal(), 1u);
  ensure_equals("invalid number of attempts", out.getAttempts(), 0u);
  ensure_equals("invalid number of writes", out.getWrites(), 0u);
}

// test writing the same node twice
template<>
template<>
void testObj::test<7>(void)
{
  nodes_.push_back( makeNewLeaf( makeNewAlert(), 123u ) );
  // first write - all's fine
  {
    const Dumper::Stats out=d_.writeToDir(nodes_, ".");
    ensure_equals("invalid number of elements 1", out.getTotal(), 1u);
    ensure_equals("invalid number of attempts 1", out.getAttempts(), 1u);
    ensure_equals("invalid number of writes 1", out.getWrites(), 1u);
  }
  // second write - unable to write duplicated node
  {
    const Dumper::Stats out=d_.writeToDir(nodes_, ".");
    ensure_equals("invalid number of elements 2", out.getTotal(), 1u);
    ensure_equals("invalid number of attempts 2", out.getAttempts(), 1u);
    ensure_equals("invalid number of writes 2", out.getWrites(), 0u);
  }
}

// test creating Stats object
template<>
template<>
void testObj::test<8>(void)
{
  const Dumper::Stats s(3u, 2u, 1u);
  ensure_equals("invalid total count", s.getTotal(), 3u);
  ensure_equals("invalid attempts count", s.getAttempts(), 2u);
  ensure_equals("invalid writes count", s.getWrites(), 1u);
}

// test if all parameters can be equal (smoke test)
template<>
template<>
void testObj::test<9>(void)
{
  Dumper::Stats s(1u, 1u, 1u);
}

// test exception when total<attempts
template<>
template<>
void testObj::test<10>(void)
{
  try
  {
    Dumper::Stats s(1u, 2u, 1u);
    fail("c-tor didn't throw on error");
  }
  catch(const std::logic_error &)
  {
    // this is expected
  }
}

// test exception when attempts<writes
template<>
template<>
void testObj::test<11>(void)
{
  try
  {
    Dumper::Stats s(1u, 1u, 2u);
    fail("c-tor didn't throw on error");
  }
  catch(const std::logic_error &)
  {
    // this is expected
  }
}

} // namespace tut
