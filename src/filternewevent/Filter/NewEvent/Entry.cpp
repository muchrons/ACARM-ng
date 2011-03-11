/*
 * Entry.cpp
 *
 */

#include <openssl/sha.h>

#include "Filter/NewEvent/Entry.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"

namespace Filter
{
namespace NewEvent
{

// TODO: this code is mostly c&p from Persistency::Facades::detail::LocalAnalyzersCache
//       make this a common code in Common component. ensure that proper library and include
//       dependencies are moved along with the implementation.


Entry::Entry(const HashSharedPtr &hashPtr, Filter::BackendFacade &bf, TimeoutedSet &ts):
  owner_("Filter::NewEvent"),
  dc_(bf.createDynamicConfig( owner_ )),
  hashPtr_( hashPtr ),
  ts_(ts)
{
  dc_->write( hashPtr_.get()->getHash(), "true");
  // TODO: notice that this object will be copyied multiple times during life cycle, thus multiple instances
  //       will be present at one time. this makes it impossible to simply add in c-tor and del in d-tor.
  //       reference counting mechanism has to be introduced for this mechanism to work.
}

Entry::~Entry()
{
  // TODO: this call may throw - add try{}catch(...), with proper comment on this event.
  ts_.add( hashPtr_ );
}

const Hash::HashData &Entry::getHash() const
{
  return hashPtr_.get()->getHash();
}

const Persistency::IO::DynamicConfig::Owner &Entry::getOwner() const
{
  return owner_;
}

bool Entry::operator==(const Entry &other) const
{
  if(getHash() != other.getHash())
    return false;
  return true;
}

} // namespace NewEvent
} // namespace Filter

