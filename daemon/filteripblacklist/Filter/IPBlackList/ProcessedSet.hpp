/*
 * ProcessedSet.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_PROCESSEDSET_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_PROCESSEDSET_HPP_FILE

#include "Base/ObservingSet.hpp"
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
  bool isProcessed(Persistency::GraphNodePtrNN leaf) const;
  void markAsProcessed(Persistency::GraphNodePtrNN leaf);
  void prune(void);

private:
  typedef Base::ObservingSet<Persistency::GraphNodePtrNN::value_type> PtrSet;

  PtrSet set_;
}; // class EntryProcessor

} // namespace IPBlackList
} // namespace Filter

#endif
