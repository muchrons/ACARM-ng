/*
 * Hash.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_HASH_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_HASH_HPP_FILE

#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/LimitedString.hpp"

namespace Filter
{
namespace NewEvent
{

class Hash
{
public:

  typedef Commons::LimitedString<64> HashData;

  Hash(const std::string &name);
  const HashData &getHash() const;
private:
  // computes (SHA1) hash of a given string
  std::string computeHash(const std::string &in) const;

  HashData hash_;
}; // class Hash

typedef Commons::SharedPtrNotNULL<Hash> HashSharedPtr;

} // namespace NewEvent
} // namespace Filter

#endif
