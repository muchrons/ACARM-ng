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
  log_("filter.newevent"),
  owner_("Filter::NewEvent"),
  dc_( bf.createDynamicConfig(owner_) ),
  hash_(hash),
  ts_(ts)
{
  // NOTE: this object will be copyied multiple times during life cycle, thus multiple instances
  //       will be present at one time. this makes it impossible to simply add in c-tor and del in d-tor.
  //       reference counting mechanism has been introduced for Entry elements stored in Processed set
  //       for this mechanism to work.
  dc_->write( getHashString(), "true");
}

Entry::~Entry()
{
  try
  {
    ts_.add(hash_);
  }
  catch(const std::exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<"exception caught while adding element to the TimeoutedSet: '"<<ex.what();
  }
}

const Hash::HashData &Entry::getHashString() const
{
  return hash_.getHash();
}

const Hash &Entry::getHash() const
{
  return hash_;
}

} // namespace NewEvent
} // namespace Filter

