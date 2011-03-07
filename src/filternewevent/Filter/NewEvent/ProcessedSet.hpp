/*
 * ProcessedSet.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_PROCESSEDSET_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_PROCESSEDSET_HPP_FILE

#include "Base/TimeoutQueue.hpp"
#include "Filter/NewEvent/Entry.hpp"

namespace Filter
{
namespace NewEvent
{

/** \brief local cache of which (meta-)alert names have been already processed (i.e. severity
 *         has been increased).
 */
class ProcessedSet
{
public:
  /** \brief checks if given entry has been alrady processed or not.
   *  \param entry entry connected with processed leaf.
   *  \return true if entry (leaf) has been already reported, false otherwise.
   */
  bool isProcessed(Entry &entry) const;
  /** \brief marks given leaf as already processed.
   *  \param entry   entry connected with processed leaf.
   *  \param timeout time life (in seconds) of stored (meta-)alert name.
   */
  void markAsProcessed(Entry &entry, unsigned int timeout);
  /** \brief removes entries corresponding to graph nodes not held in memory any more.
   */
  void prune(void);
private:

  typedef Base::TimeoutQueue<Entry::Name> Set;

  void pruneTimeouted();

  Set set_;

}; // class ProcessedSet
} // namespace Filter
} // namespace NewEvent
#endif
