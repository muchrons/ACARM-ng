/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
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

  void testFileInRange(const char *root, const time_t from, const time_t to) const
  {
    const char *error="no elements in range";
    for(time_t t=from; t<=to; ++t)
    {
      // in-test quick-hack
      switch( testFile(root, t, 0) )
      {
        case 0:
          error=NULL;
          break;
        case 1:
          error="no such file";
          break;
        case 2:
          error="file's too small (empty?)";
          break;
        default:
          assert(!"unhandled return code");
      } // switch(file_existance)
    } // for(timestamp-range)

    if(error!=NULL)
      tut::fail(error);
  }

  int testFile(const char *root, const time_t ts, unsigned int index) const
  {
    assert(root!=NULL);
    stringstream ss;
    ss<<root<<"/"<<ts<<"_"<<index<<".txt";
    struct stat buf;
    if( stat( ss.str().c_str(), &buf )!=0 )
      return 1;
    if( buf.st_size<40 )
      return 2;
    // all ok!
    return 0;
  }

  int testMultipleWrites(void) const
  {
    Strategy               s(cfg_);
    Strategy::ChangedNodes nc;
    GraphNodePtrNN         node=makeNewNode();

    // wait until seconds just changed - to make event as probable as possible
    // (yes - this is busy loop...)
    {
      time_t start=time(NULL);
      while( start==time(NULL) ) {}
    }

    // no make double write
    const time_t start=time(NULL);
    s.process(node, nc);
    s.process(node, nc);
    const time_t stop =time(NULL);
    // if second elapsed there is nothing that can be done
    if(start!=stop)
      return -1;

    // check for output files
    testFile(".", start, 0);
    testFile(".", start, 1);
    return 0;
  }

  const Config cfg_;    // default config
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/File/Strategy");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  const Strategy s(cfg_);
  ensure_equals("invalid name", s.getTriggerName(), "file");
}

// test saving report in current directory
template<>
template<>
void testObj::test<2>(void)
{
  Strategy               s(cfg_);
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
  Strategy               s(cfg);
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
    if( testMultipleWrites()==0 )   // ok - we're done now
      return;
  // 5 fails means FAIL...
  fail("this test's main part has to fit in <1[s] time window and failed "
       "to do so 5 times. looks like your machine is to slow to perform "
       "this test - aborting");
}

} // namespace tut
