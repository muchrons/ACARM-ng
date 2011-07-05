/*
 * Hash.cpp
 *
 */


#include "Commons/computeHash.hpp"
#include "Filter/NewEvent/Entry.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"

namespace Filter
{
namespace NewEvent
{

Hash::Hash(const std::string &name):
  hash_( Commons::computeHash(name) )
{
}

const Hash::HashData &Hash::getHash() const
{
  return hash_;
}

bool Hash::operator==(const Hash &other) const
{
  if(getHash() != other.getHash())
    return false;
  return true;
}

} // namespace NewEvent
} // namespace Filter
