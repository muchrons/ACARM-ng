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

/** \brief 
 */
class ProcessedSet
{
public:
  bool isProcessed(Persistency::GraphNodePtrNN leaf, Persistency::HostPtrNN host) const;
  void markAsProcessed(Persistency::GraphNodePtrNN leaf, Persistency::HostPtrNN host);
  void prune(void);

private:
  typedef Base::ObservingPairSet<Persistency::GraphNodePtrNN::value_type,
                                 Persistency::HostPtrNN>                  PtrSet;

  PtrSet set_;
}; // class EntryProcessor

} // namespace IPBlackList
} // namespace Filter

#endif
