/*
 * Entry.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE

#include "Logger/Node.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Filter/BackendFacade.hpp"
#include "Filter/NewEvent/Exception.hpp"
#include "Filter/NewEvent/Hash.hpp"

namespace Filter
{
namespace NewEvent
{

class TimeoutedSet;

/** \brief
 *  save elements in the persistency dynamic config,
 *  save timeouted elements in dedicated collection.
 */
class Entry: private boost::noncopyable
{
public:
  // TODO: do not refer to the specific hash algorithm - it is not given explicitly
  //       by design, so that it could be easily changed any time, when needed.

  /** \brief create instance.
   *  \param hash SHA1 hash of processes (meta-)alert name.
   *  \param bf   facade for saving elements in the persistency.
   *  \param ts   cache for storing timeouted elements.
   */
  Entry(const Hash &hash, Filter::BackendFacade &bf, TimeoutedSet &ts);
  /** \brief delete instance and add hash of timeouted element to the cache.
   */
  ~Entry();

  /** \brief return hash string of entry name.
   *  \return (SHA1) hash string of entry name.
   */
  const Hash::HashData &getHashString() const;
  /** \brief return hash of entry name.
   *  \return (SHA1) hash of entry name.
   */
  const Hash &getHash() const;
private:

  Logger::Node                                               log_;
  Persistency::IO::DynamicConfig::Owner                      owner_;
  Commons::SharedPtrNotNULL<Persistency::IO::DynamicConfig>  dc_;
  Hash                                                       hash_;
  TimeoutedSet                                               &ts_;
}; // class Entry

/** \brief smart pointer to Entry type, that is sure not to be NULL. */
typedef Commons::SharedPtrNotNULL<Entry> EntrySharedPtr;

} // namespace Filter
} // namespace NewEvent

#endif
