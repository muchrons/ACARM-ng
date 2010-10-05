/*
 * IDAssigner.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/scoped_ptr.hpp>

#include "Commons/Convert.hpp"
#include "Persistency/Facades/IDAssigner.hpp"
#include "Persistency/IO/IOStubs.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades;

namespace
{

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
  virtual Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const Persistency::IO::DynamicConfig::Owner &owner,
                                                                  Persistency::IO::Transaction                &t)
  {
    tut::ensure_equals("invalid owner", owner.get(), std::string("Persistency::IDAssigner") );
    if( ioCounter_.get()==NULL )
      ioCounter_.reset( new IODynamicConfigCounter(t, "next free MetaAlert's ID") );
    tut::ensure("NULL ioCounter_ - logic error", ioCounter_.get()!=NULL );
    return Persistency::IO::DynamicConfigAutoPtr( new DynamicConfigCounter(*ioCounter_, owner, t) );
  }

  boost::scoped_ptr<IODynamicConfigCounter> ioCounter_;
}; // struct TestIOConnectionCounter


IO::ConnectionPtrNN::SharedPtr g_conn(new TestIOConnectionCounter);

struct TestClass
{
  TestClass(void):
    t_( g_conn->createNewTransaction("test_assigner") ),
    startID_( IDAssigner::get()->assign(g_conn, t_).get() )
  {
  }

  Persistency::IO::DynamicConfigAutoPtr getDC(void)
  {
    return g_conn->dynamicConfig("Persistency::IDAssigner", t_);
  }

  IO::Transaction        t_;
  MetaAlert::ID::Numeric startID_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/IDAssigner");
} // unnamed namespace


namespace tut
{

// test getting when no value is set value
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid value returned", IDAssigner::get()->assign(g_conn, t_).get(), startID_+1u);
}

// test reading multiple times
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid value 1 returned", IDAssigner::get()->assign(g_conn, t_).get(), startID_+1u);
  ensure_equals("invalid value 2 returned", IDAssigner::get()->assign(g_conn, t_).get(), startID_+2u);
}

} // namespace tut
