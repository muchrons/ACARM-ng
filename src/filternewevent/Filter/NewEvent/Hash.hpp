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

/** \brief holds hash of processed (meta-)alert name.
 */

class Hash
{
public:

  /** \brief hash type. */
  typedef Commons::LimitedString<64> HashData;
  /** \brief create instance.
   *  \param name name for which SHA1 hash will be computed.
   */
  Hash(const std::string &name);
  /** \brief return stored SHA1 hash.
   *  \return SHA1 hash.
   */
  const HashData &getHash() const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Hash &other) const;
private:

  HashData hash_;
}; // class Hash

typedef Commons::SharedPtrNotNULL<Hash> HashSharedPtr;

} // namespace NewEvent
} // namespace Filter

#endif
