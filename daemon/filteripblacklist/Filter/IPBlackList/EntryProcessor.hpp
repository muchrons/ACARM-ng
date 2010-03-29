/*
 * EntryProcessor.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_ENTRYPROCESSOR_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_ENTRYPROCESSOR_HPP_FILE

#include <cassert>

#include "Persistency/GraphNode.hpp"
#include "Filter/BackendFacade.hpp"
#include "Filter/IPBlackList/BlackList.hpp"

namespace Filter
{
namespace IPBlackList
{

/** \brief helper object for processing given leaf.
 *
 *  processing in this context means reverse mapping IPs to DNS names
 *  for each host (source or destination) attached to given leaf.
 */
class EntryProcessor
{
public:
  /** \brief create instance.
   *  \param bl       black list of hosts.
   *  \param bf       facade for writing new names of hosts to persistency.
   *  \param priDelta change of priority when black-listed host is found.
   */
  EntryProcessor(const BlackList *bl, BackendFacade *bf, double priDelta);
  /** \brief method responsible for doing all the job.
   *  \param leaf leaft to be processed.
   */
  void operator()(Persistency::GraphNodePtrNN leaf);

private:
  void processHosts(Persistency::GraphNodePtrNN              leaf,
                    const Persistency::Alert::ReportedHosts &rh);

  const BlackList *bl_;
  BackendFacade   *bf_;
  double           priDelta_;
}; // class EntryProcessor

} // namespace IPBlackList
} // namespace Filter

#endif
