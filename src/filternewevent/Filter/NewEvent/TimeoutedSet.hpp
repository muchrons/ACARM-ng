/*
 * TimeoutedSet.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_TIMEOUTEDSET_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_TIMEOUTEDSET_HPP_FILE

#include "Filter/BackendFacade.hpp"
#include "Filter/Exception.hpp"
#include "Filter/NewEvent/Entry.hpp"

namespace Filter
{
namespace NewEvent
{

/** \brief local cache to store timeouted elements.
 *
 */
class TimeoutedSet
{
private:
  typedef std::vector<Entry::Hash> Timeouted;

public:
  //TODO: parameter must be a const-ref
  /** \brief add timeouted key to the collection.
   *  \param key key to be added.
   */
  void add(const Entry::Hash &key);
  // TODO: 'owner' must be a const-ref.
  // TODO: take 'bf' as ref and the address of it in the implemenation to avaid NULLs.
  // TODO: 'prune' is not the happiest name for this method. consider something like markRemoved() or similar.
  /** \brief remove timeouted element from collection and from persistency.
   *  \param bf    facade for removing timeouting elements from persistency.
   *  \param owner owner's name.
   */
  void prune(BackendFacade *bf, Persistency::IO::DynamicConfig::Owner &owner);
  // TODO: 'key' must be NULL.
  // TODO: this method is never used and should be removed
  /** \brief checks if given key has been alrady timeouted or not.
   *  \param key key from which ...
   *  \return true if key has been already timeouted, false otherwise.
   */
  bool isTimeouted(const Entry::Hash &key) const;

private:
  Timeouted timeouted_;
}; // class TimeoutedSet

} // namespace Filter
} // namespace NewEvent

#endif
