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

// TODO: do not add doxygen for forward declarations.
/** \brief forward declaration of TimeoutedSet class.
 */
class TimeoutedSet;

// TODO: this class must be non-copyable.
/** \brief
 *  save elements in the persistency dynamic config,
 *  save timeouted elements in dedicated collection.
 */
class Entry
{
public:

  /** \brief create instance.
   *  \param hash SHA1 hash of processes (meta-)alert name.
   *  \param bf   facade for saving elements in the persistency.
   *  \param ts   cache for storing timeouted elements.
   */
  Entry(const Hash &hash, Filter::BackendFacade &bf, TimeoutedSet &ts);
  /** \brief delete instance and add hash of timeouted element to the cache.
   */
  ~Entry();

  /** \brief return hash of entry name.
   *  \return (SHA1) hash of entry name.
   */
  const Hash::HashData &getHash() const;
private:

  Logger::Node                                               log_;
  Persistency::IO::DynamicConfig::Owner                      owner_;
  Commons::SharedPtrNotNULL<Persistency::IO::DynamicConfig>  dc_;
  Hash                                                       hash_;
  TimeoutedSet                                              *ts_;   // TODO: since class is non-copyable use ref instead of ptr (not !NULL check required)
}; // class Entry

// TODO: doxygen is missing
typedef Commons::SharedPtrNotNULL<Entry> EntrySharedPtr;

} // namespace Filter
} // namespace NewEvent

#endif
