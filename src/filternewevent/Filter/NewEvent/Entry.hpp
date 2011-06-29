/*
 * Entry.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE

#include "Commons/SharedPtrNotNULL.hpp"
#include "Filter/BackendFacade.hpp"
#include "Filter/NewEvent/Exception.hpp"
#include "Filter/NewEvent/Hash.hpp"

namespace Filter
{
namespace NewEvent
{

/** \brief forward declaration of TimeoutedSet class.
 */
class TimeoutedSet;
/** \brief
 *  save elements in the persistency dynamic config,
 *  save timeouted elements in dedicated collection.
 */
class Entry
{
public:

  /** \brief create instance.
   *  \param hashPtr SHA1 hash of processes (meta-)alert name.
   *  \param bf      facade for saving elements in the persistency.
   *  \param ts      cache for storing timeouted elements.
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

  Persistency::IO::DynamicConfig::Owner                      owner_;
  Commons::SharedPtrNotNULL<Persistency::IO::DynamicConfig>  dc_;
  Hash                                                       hash_;
  TimeoutedSet                                              *ts_;
}; // class Entry

typedef Commons::SharedPtrNotNULL<Entry> EntrySharedPtr;

} // namespace Filter
} // namespace NewEvent

#endif
