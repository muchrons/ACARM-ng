/*
 * ConnectionIOCounter.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_CONNECTIONIOCOUNTER_HPP_FILE
#define INCLUDE_TESTHELPERS_PERSISTENCY_CONNECTIONIOCOUNTER_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <cassert>

#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"
#include "TestHelpers/Persistency/IODynamicConfigCounter.hpp"

namespace TestHelpers
{
namespace Persistency
{

// TODO: refactor this using IODynamicConfigCounter

// helper typedef
typedef boost::shared_ptr<IODynamicConfigCounter> IODynamicConfigCounterPtr;

// counter that forwards all calls to other counter, that does not change
struct DynamicConfigCounter: public ::Persistency::IO::DynamicConfig
{
  DynamicConfigCounter(IODynamicConfigCounterPtr counter, const Owner &owner, ::Persistency::IO::Transaction &t):
    ::Persistency::IO::DynamicConfig(owner, t),
    counter_(counter)
  {
    assert( counter_.get()!=NULL );
  }

  virtual void writeImpl(::Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
  {
    counter_->write(key, value);
  }

  virtual ValueNULL readImpl(::Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    return counter_->read(key);
  }

  virtual Value readConstImpl(::Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    return counter_->readConst(key);
  }

  virtual void removeImpl(::Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    counter_->remove(key);
  }

  virtual void iterateImpl(::Persistency::IO::Transaction &/*t*/, IterationCallback &cb)
  {
    counter_->iterate(cb);
  }

  IODynamicConfigCounterPtr counter_;
}; // class DynamicConfigCounter


// connection that returns different implementaitons of the same counter
struct ConnectionIOCounter: public ConnectionUserStubBase
{
  // key - key name that will be used for the counter
  ConnectionIOCounter(const std::string &owner, const std::string &key):
    owner_(owner),
    key_(key)
  {
  }

  virtual ::Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const ::Persistency::IO::DynamicConfig::Owner &owner,
                                                                    ::Persistency::IO::Transaction                &t)
  {
    tut::ensure_equals("invalid owner", owner.get(), owner_);
    if( ioCounter_.get()==NULL )
      ioCounter_.reset( new IODynamicConfigCounter(t, key_) );
    tut::ensure("NULL ioCounter_ - logic error", ioCounter_.get()!=NULL );
    return ::Persistency::IO::DynamicConfigAutoPtr( new DynamicConfigCounter(ioCounter_, owner, t) );
  }

  const std::string         owner_;
  const std::string         key_;
  IODynamicConfigCounterPtr ioCounter_;
}; // struct ConnectionIOCounter

} // namespace Persistency
} // namespace TestHelpers

#endif
