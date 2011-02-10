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

/** \brief local cache of which leaf have been already processed (i.e. severity
 *         has been increased).
 */
class ProcessedSet
{
public:
  /** \brief checks if given leaf has been alrady processed or not.
   *  \param leaf leaf from which host is from.
   *  \return true if leaf has been already reported, false otherwise.
   */
  bool isProcessed(Entry &entry) const;
  /** \brief marks given leaf as already processed.
   *  \param leaf leaf from which host is from.
   */
  void markAsProcessed(Entry &entry, unsigned int timeout);
  /** \brief removes entries corresponding to graph nodes not held in memory any more.
   */
  void prune(void);
private:

  typedef Base::TimeoutQueue<std::string> Set;

  Set set_;

}; // class ProcessedSet
} // namespace Filter
} // namespace NewEvent
#endif
