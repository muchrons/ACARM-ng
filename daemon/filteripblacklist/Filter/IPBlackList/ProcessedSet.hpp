/*
 * ProcessedSet.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_PROCESSEDSET_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_PROCESSEDSET_HPP_FILE

#include "Base/ObservingPairSet.hpp"
#include "Persistency/GraphNode.hpp"
#include "Filter/BackendFacade.hpp"
#include "Filter/IPBlackList/BlackList.hpp"

namespace Filter
{
namespace IPBlackList
{

/** \brief local cache of which leaf:host pairs have been already processed (i.e. severity
 *         has been increased).
 */
class ProcessedSet
{
public:
  /** \brief checks if given pair has been alrady processed or not.
   *  \param leaf leaf from which host is from.
   *  \param host host assigned to given leaf.
   *  \return true if pair has been already reported, false otherwise.
   */
  bool isProcessed(Persistency::GraphNodePtrNN leaf, Persistency::HostPtrNN host) const;
  /** \brief marks given pair as already processed.
   *  \param leaf leaf from which host is from.
   *  \param host host assigned to given leaf.
   */
  void markAsProcessed(Persistency::GraphNodePtrNN leaf, Persistency::HostPtrNN host);
  /** \brief removes entries corresponding to graph nodes not held in memory any more.
   */
  void prune(void);

private:
  typedef Base::ObservingPairSet<Persistency::GraphNodePtrNN::value_type,
                                 Persistency::HostPtrNN>                  PtrSet;

  PtrSet set_;
}; // class EntryProcessor

} // namespace IPBlackList
} // namespace Filter

#endif
