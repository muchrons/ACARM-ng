/*
 * EntryProcessor.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_ENTRYPROCESSOR_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_ENTRYPROCESSOR_HPP_FILE

#include "Logger/Node.hpp"
#include "Persistency/GraphNode.hpp"
#include "Filter/BackendFacade.hpp"
#include "Filter/NewEvent/Strategy.hpp"
#include "Filter/NewEvent/ProcessedSet.hpp"

namespace Filter
{
namespace NewEvent
{

/** \brief helper object for processing given leaf.
 *
 */
class EntryProcessor
{
public:
  /** \brief create instance.
   *  \param bf       facade for writing new names of hosts to persistency.
   *  \param ps       set of already processed elements.
   *  \param ts       set of already timeouted elements.
   *  \param params   parameters (timeout and priority delta).
   */
  EntryProcessor(BackendFacade *bf, ProcessedSet *ps, TimeoutedSet *ts, const Strategy::Parameters params);
  /** \brief method responsible for doing all the job.
   *  \param leaf leaft to be processed.
   */
  void operator()(Persistency::GraphNodePtrNN leaf);

private:

  Logger::Node               log_;
  BackendFacade             *bf_;
  ProcessedSet              *ps_;
  TimeoutedSet              *ts_;
  const Strategy::Parameters params_;
}; // class EntryProcessor

} // namespace IPBlackList
} // namespace Filter
#endif
