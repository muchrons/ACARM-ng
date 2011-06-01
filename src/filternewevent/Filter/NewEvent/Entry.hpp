/*
 * Entry.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE

#include "Commons/SharedPtrNotNULL.hpp"
#include "Filter/BackendFacade.hpp"
// TODO: according to convention Filter::NewEvent must have its own exception type, derived
//       from Filter::Exception, but cannot throw Filter::Exception directly in own code.
#include "Filter/Exception.hpp"

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
  Entry(const HashSharedPtr &hashPtr, Filter::BackendFacade &bf, TimeoutedSet &ts);
  /** \brief delete instance and add hash of timeouted element to the cache.
   */
  ~Entry();

  /** \brief return hash of entry name.
   *  \return (SHA1) hash of entry name.
   */
  const Hash::HashData &getHash() const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Entry &other) const;
private:

  Persistency::IO::DynamicConfig::Owner                      owner_;
  Commons::SharedPtrNotNULL<Persistency::IO::DynamicConfig>  dc_;
  HashSharedPtr                                              hashPtr_;
  TimeoutedSet                                              *ts_;
}; // class Entry

typedef Commons::SharedPtrNotNULL<Entry> EntrySharedPtr;

} // namespace Filter
} // namespace NewEvent

#endif
