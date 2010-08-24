/*
 * IDAssigner.cpp
 *
 */
#include <cassert>

#include "Commons/Convert.hpp"
#include "Persistency/IDAssigner.hpp"

using Persistency::IO::DynamicConfig;

namespace
{
const char *key="next free MetaAlert's ID";
} // unnamed namespace

namespace Persistency
{

IDAssigner::IDAssigner(IO::ConnectionPtrNN conn, IO::Transaction &t):
  conn_(conn),
  t_(t),
  dc_( conn_->dynamicConfig("Persistency::IDAssigner", t_) )
{
  assert( dc_.get()!=NULL );
}

MetaAlert::ID IDAssigner::assign(void)
{
  MetaAlert::ID                  freeID(0u);
  const DynamicConfig::ValueNULL r=dc_->read(key);
  // if value is set, counter is already started
  if( r.get()!=NULL )
  {
    // parse input value and return to caller
    typedef MetaAlert::ID::Numeric NumericID;
    freeID=Commons::Convert::to<NumericID>( r.get()->get() );
  }

  // save next free ID
  dc_->write(key, Commons::Convert::to<std::string>( freeID.get()+1u ) );

  // return final value.
  return freeID;
}

} // namespace Persistency
