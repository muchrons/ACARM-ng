/*
 * IDAssignerDynamic.cpp
 *
 */
#include "Commons/Convert.hpp"
#include "Persistency/IDAssignerDynamic.hpp"

using Persistency::IO::DynamicConfig;

namespace
{
const char *key="next free GraphNode's ID";
} // unnamed namespace

namespace Persistency
{

GraphNode::ID IDAssignerDynamic::assign(IO::DynamicConfig &dc)
{
  GraphNode::ID                  freeID(0u);
  const DynamicConfig::ValueNULL r=dc.read(key);
  // if value is set, counter is already started
  if( r.get()!=NULL )
  {
    // parse input value and return to caller
    typedef GraphNode::ID::Numeric NumericID;
    freeID=Commons::Convert::to<NumericID>( r.get()->get() );
  }

  // save next free ID
  dc.write(key, Commons::Convert::to<std::string>( freeID.get()+1u ) );

  return freeID;
}

} // namespace Persistency
