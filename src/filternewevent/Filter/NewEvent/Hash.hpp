/*
 * Hash.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_HASH_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_HASH_HPP_FILE

#include <boost/operators.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/LimitedString.hpp"
#include "Persistency/IO/DynamicConfig.hpp"

namespace Filter
{
namespace NewEvent
{

/** \brief holds hash of processed (meta-)alert name.
 */
class Hash: boost::equality_comparable<Hash>
{
public:
  /** \brief hash type. */
  typedef Persistency::IO::DynamicConfig::Key HashData;

  /** \brief create instance.
   *  \param name name for which SHA1 hash will be computed.
   */
  explicit Hash(const std::string &name);
  /** \brief return stored SHA1 hash.
   *  \return SHA1 hash.
   */
  const HashData &getHash() const
  {
    return hash_;
  }
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Hash &other) const
  {
    return getHash()==other.getHash();
  }

private:
  HashData hash_;
}; // class Hash

/** \brief smart pointer to Hash type, that is sure not to be NULL. */
typedef Commons::SharedPtrNotNULL<Hash> HashSharedPtr;

} // namespace NewEvent
} // namespace Filter

#endif
