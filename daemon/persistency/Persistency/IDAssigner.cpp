/*
 * IDAssigner.cpp
 *
 */
#include <cassert>

#include "System/Threads/SafeInitLocking.hpp"
#include "Base/Threads/Lock.hpp"
#include "Logger/Logger.hpp"
#include "Commons/Convert.hpp"
#include "Persistency/IDAssigner.hpp"

using Persistency::IO::DynamicConfig;

namespace
{
const char *owner="Persistency::IDAssigner";
const char *key  ="next free MetaAlert's ID";
} // unnamed namespace

namespace Persistency
{
namespace detail
{

IDAssigner::IDAssigner(IO::ConnectionPtrNN conn, IO::Transaction &t):
  log_("persistency.idassigner")
{
  IO::DynamicConfigAutoPtr dc=conn->dynamicConfig(owner, t);
  assert( dc.get()!=NULL );

  // read last value from data base
  nextFreeID_=0u;
  LOGMSG_DEBUG(log_, "reading last value of counter");
  const DynamicConfig::ValueNULL r=dc->read(key);
  // if value is set, counter is already started
  if( r.get()!=NULL )
  {
    LOGMSG_DEBUG_S(log_)<<"value already set to: "<<r.get()->get();
    // parse input value and return to caller
    typedef MetaAlert::ID::Numeric NumericID;
    nextFreeID_=Commons::Convert::to<NumericID>( r.get()->get() );
  }
  else
    LOGMSG_DEBUG(log_, "value not set yet - starting counting with 0");
  LOGMSG_DEBUG_S(log_)<<"first free meta-alert ID is "<<nextFreeID_;
}

MetaAlert::ID IDAssigner::assign(IO::ConnectionPtrNN conn, IO::Transaction &t)
{
  LOGMSG_DEBUG(log_, "assigning next ID value");
  const MetaAlert::ID assignedID=nextFreeID_;   // save assigned value
  ++nextFreeID_;                                // move to next one
  IO::DynamicConfigAutoPtr dc=conn->dynamicConfig(owner, t);
  assert( dc.get()!=NULL );
  // save next free ID
  dc->write(key, Commons::Convert::to<std::string>(nextFreeID_) );
  LOGMSG_DEBUG_S(log_)<<"assigned value is "<<assignedID.get();
  LOGMSG_DEBUG_S(log_)<<"next free value is "<<nextFreeID_;

  // return assigned value
  return assignedID;
}



MetaAlert::ID IDAssignerWrapper::assign(IO::ConnectionPtrNN conn, IO::Transaction &t)
{
  Base::Threads::Lock lock(mutex_);
  if( impl_.get()==NULL )
    impl_.reset( new IDAssigner(conn, t) );
  assert( impl_.get()!=NULL );
  return impl_->assign(conn, t);
}

} // namespace detail
} // namespace Persistency
