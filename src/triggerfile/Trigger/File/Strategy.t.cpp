/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <stack>
#include <sstream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>

#include "Trigger/File/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Core::Types::Proc;
using namespace Trigger::File;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( ".", Trigger::Simple::ThresholdConfig("1.2", "2") )
  {
  }

  ~TestClass(void)
  {
    while( !rmThis_.empty() )
    {
      try
      {
        unlink( rmThis_.top().c_str() );
        rmdir ( rmThis_.top().c_str() );
        rmThis_.pop();
      }
      catch(...)
      {
        assert(!"exception in d-tor");
      }
    } // while(!empty)
  }

  void testFileInRange(const char *root, const time_t from, const time_t to)
  {
    for(time_t t=from; t<=to; ++t)
    {
      // in-test quick-hack
      switch( testFile(root, t, 0) )
      {
        case 0:
          return;
        case 1:
          // no such file - check for next one
          break;
        case 2:
          tut::fail("file's size is invalid (empty?)");
          break;
        default:
          assert(!"unhandled return code");
      } // switch(file_existance)
    } // for(timestamp-range)

    tut::fail("no elements in range");
  }

  int testFile(const char *root, const time_t ts, unsigned int index)
  {
    assert(root!=NULL);
    stringstream ss;
    ss<<root<<"/"<<ts<<"_"<<index<<".txt";
    rmThis_.push( root     );
    rmThis_.push( ss.str() );
    struct stat buf;
    if( stat( ss.str().c_str(), &buf )!=0 )
      return 1;
    if( buf.st_size<135 )
      return 2;
    // all ok!
    return 0;
  }

  const Config   cfg_;      // default config
  stack<string> rmThis_;   // list of elements to be removed after test finishes
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/File/Strategy");
} // unnamed namespace


namespace tut
{

// check if trigger type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  const Strategy s(InstanceName("mytrigger"), cfg_);
  ensure_equals("invalid trigger type", s.getTriggerType().str(), "file");
}

// test saving report in current directory
template<>
template<>
void testObj::test<2>(void)
{
  Strategy               s(InstanceName("myfiletrigger"), cfg_);
  Strategy::ChangedNodes nc;
  const time_t           start=time(NULL);
  s.process( makeNewNode(), nc );
  const time_t           stop =time(NULL);
  testFileInRange(".", start, stop);
}

// test saving report in other directory
template<>
template<>
void testObj::test<3>(void)
{
  const char *out="TEST_OUT_DIR";
  if( mkdir(out, 0755)!=0 )
    ensure("unable to create output dir", errno==EEXIST);
  const Config           cfg(out, cfg_.getThresholdConfig() );
  Strategy               s(InstanceName("myfiletrigger"), cfg);
  Strategy::ChangedNodes nc;
  const time_t           start=time(NULL);
  s.process( makeNewNode(), nc );
  const time_t           stop =time(NULL);
  testFileInRange(out, start, stop);
}

// test saving multiple reports within one second
template<>
template<>
void testObj::test<4>(void)
{
  // this test main part has to fit in <1[s] window - retru 5 times
  // if it does not happen
  for(int i=0; i<5; ++i)
  {
    Strategy               s(InstanceName("myfiletrigger"), cfg_);
    Strategy::ChangedNodes nc;

    // wait until seconds just changed - to make event as probable as possible
    // (yes - this is busy loop...)
    {
      time_t start=time(NULL);
      while( start==time(NULL) )
      {
      }
    }

    // no make double write
    const time_t start=time(NULL);
    s.process( makeNewNode(), nc );
    s.process( makeNewNode(), nc );
    const time_t stop =time(NULL);
    // if second elapsed there is nothing that can be done
    if(start!=stop)
      continue;

    // check for output files
    ensure("first output file does not exist",  testFile(".", start, 0)==0 );
    ensure("second output file does not exist", testFile(".", start, 1)==0 );
    // ok - if we're here, test succeeded.
    return;
  }

  // 5 fails means FAIL...
  fail("this test's main part has to fit in <1[s] time window and failed "
       "to do so 5 times. looks like your machine is to slow to perform "
       "this test - aborting");
}

} // namespace tut
