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
  void markAsProcessed(EntrySharedPtr entry, unsigned int timeout);
  /** \brief removes entries corresponding to graph nodes not held in memory any more.
   */
  void prune(void);
private:

  // TODO: replace with boost::ptr_map in order to provide memory free when element is deleted
  //       from collection. This will be usefull for deleting elements stored in persistency.
  //       Functionality for deleting timeouted elemnts should be provided.
  typedef Base::TimeoutQueue<EntrySharedPtr> Set;

  void pruneTimeouted();

  Set set_;

}; // class ProcessedSet
} // namespace Filter
} // namespace NewEvent
#endif
