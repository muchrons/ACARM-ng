/*
 * IDAssigner.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <cassert>

#include "Commons/Convert.hpp"
#include "Persistency/Facades/IDAssigner.hpp"
#include "Persistency/IO/IOStubs.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades;

namespace
{

enum ExpectedKey
{
  EXPK_UNKNOWN  =0,
  EXPK_METAALERT=1,
  EXPK_ANALYZER =2
};

ExpectedKey g_expectedKey=EXPK_UNKNOWN;

const char *toString(ExpectedKey e)
{
  switch(e)
  {
    case EXPK_METAALERT: return "next free MetaAlert's ID";
    case EXPK_ANALYZER:  return "next free Analyzer's ID";
    default:
      assert(!"unknown ID string requsted - code not updated?");
      break;
  }
} // toString()

// counter that forwards all calls to other counter
struct DynamicConfigCounter: public IO::DynamicConfig
{
  DynamicConfigCounter(IODynamicConfigCounter &counter, const Owner &owner, IO::Transaction &t):
    IO::DynamicConfig(owner, t),
    counter_(counter)
  {
  }

  virtual void writeImpl(IO::Transaction &/*t*/, const Key &key, const Value &value)
  {
    counter_.write(key, value);
  }

  virtual ValueNULL readImpl(IO::Transaction &/*t*/, const Key &key)
  {
    return counter_.read(key);
  }

  virtual Value readConstImpl(IO::Transaction &/*t*/, const Key &key)
  {
    return counter_.readConst(key);
  }

  IODynamicConfigCounter &counter_;
}; // class DynamicConfig


// connection that returns different implementaitons of the same counter
struct TestIOConnectionCounter: public TestIOConnection
{
  TestIOConnectionCounter(void):
    cnt_(0)
  {
  }

  virtual Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const Persistency::IO::DynamicConfig::Owner &owner,
                                                                  Persistency::IO::Transaction                &t)
  {
    tut::ensure_equals("invalid owner", owner.get(), std::string("Persistency::IDAssigner") );

    if( ioCounterMetaAlert_.get()==NULL )
      ioCounterMetaAlert_.reset( new IODynamicConfigCounter(t, toString(EXPK_METAALERT) ) );

    if( ioCounterAnalyzer_.get()==NULL )
      ioCounterAnalyzer_.reset( new IODynamicConfigCounter(t, toString(EXPK_ANALYZER) ) );

    IODynamicConfigCounter *tmp=NULL;
    switch(g_expectedKey)
    {
      case EXPK_METAALERT: tmp=ioCounterMetaAlert_.get(); break;
      case EXPK_ANALYZER:  tmp=ioCounterAnalyzer_.get();  break;
      default: tut::fail("expected key type is not set");
    }
    switch(++cnt_)
    {
      case 1: tmp=ioCounterMetaAlert_.get(); break;
      case 2: tmp=ioCounterAnalyzer_.get();  break;
      default:                               break;
    }

    tut::ensure("login error - 'tmp' is NULL", tmp!=NULL);
    return Persistency::IO::DynamicConfigAutoPtr( new DynamicConfigCounter(*tmp, owner, t) );
  }

  int                                       cnt_;
  boost::scoped_ptr<IODynamicConfigCounter> ioCounterMetaAlert_;
  boost::scoped_ptr<IODynamicConfigCounter> ioCounterAnalyzer_;
}; // struct TestIOConnectionCounter



struct TestClass
{
  TestClass(void):
    conn_(new TestIOConnectionCounter),
    t_( conn_->createNewTransaction("test_assigner") )
  {
  }

  IO::ConnectionPtrNN         conn_;
  IO::Transaction             t_;
  Facades::detail::IDAssigner ida_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/IDAssigner");
} // unnamed namespace


namespace tut
{

// test getting some value from singleton (smoke-test)
template<>
template<>
void testObj::test<1>(void)
{
  g_expectedKey=EXPK_METAALERT;
  ensure_equals("invalid value returned", IDAssigner::get()->assignMetaAlertID(conn_, t_).get(), 0u);
}

// test getting meta-alert's ID when no value is set value
template<>
template<>
void testObj::test<2>(void)
{
  g_expectedKey=EXPK_METAALERT;
  ensure_equals("invalid value returned", ida_.assignMetaAlertID(conn_, t_).get(), 0u);
}

// test assigning meta-alerts' ID multiple times
template<>
template<>
void testObj::test<3>(void)
{
  g_expectedKey=EXPK_METAALERT;
  ensure_equals("invalid value 1 returned", ida_.assignMetaAlertID(conn_, t_).get(), 0u);
  ensure_equals("invalid value 2 returned", ida_.assignMetaAlertID(conn_, t_).get(), 1u);
}

// test getting analyzer's id when no value is set value
template<>
template<>
void testObj::test<4>(void)
{
  g_expectedKey=EXPK_ANALYZER;
  ensure_equals("invalid value returned", ida_.assignAnalyzerID(conn_, t_).get(), 0u);
}

// test assigning analyzers' ID multiple times
template<>
template<>
void testObj::test<5>(void)
{
  g_expectedKey=EXPK_ANALYZER;
  ensure_equals("invalid value 1 returned", ida_.assignAnalyzerID(conn_, t_).get(), 0u);
  ensure_equals("invalid value 2 returned", ida_.assignAnalyzerID(conn_, t_).get(), 1u);
}

} // namespace tut
