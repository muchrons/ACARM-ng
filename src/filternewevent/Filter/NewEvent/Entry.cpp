/*
 * Entry.cpp
 *
 */

#include <openssl/sha.h>
#include <iostream>
#include "Filter/NewEvent/Entry.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"

namespace Filter
{
namespace NewEvent
{


Entry::Entry(const Hash &hash, Filter::BackendFacade &bf, TimeoutedSet &ts):
  owner_("Filter::NewEvent"),
  dc_(bf.createDynamicConfig( owner_ )),
  hash_( hash ),
  ts_(&ts)
{
  dc_->write( hash_.getHash(), "true");
  // TODO: notice that this object will be copyied multiple times during life cycle, thus multiple instances
  //       will be present at one time. this makes it impossible to simply add in c-tor and del in d-tor.
  //       reference counting mechanism has to be introduced for this mechanism to work.
}

Entry::~Entry()
{
  // TODO: this call may throw - add try{}catch(...), with proper comment on this event.
  ts_->add( hash_ );
}

const Hash::HashData &Entry::getHash() const
{
  return hash_.getHash();
}

} // namespace NewEvent
} // namespace Filter

