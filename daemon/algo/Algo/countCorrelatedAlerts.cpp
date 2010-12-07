/*
 * countCorrelatedAlerts.cpp
 *
 */
#include <set>

#include "Algo/countCorrelatedAlerts.hpp"
#include "Algo/forEachInTree.hpp"

namespace Algo
{

namespace
{
/** \brief helper object counting number of alerts in given (sub)tree.
 */
class AlertsCounter
{
public:
  /** \brief work procedure itself.
   *  \param node node to count (if it is alert)
   */
  void operator()(Persistency::ConstGraphNodePtrNN node)
  {
    if( node->isLeaf() )
      set_.insert( node.get() );        // keep the pointer - it is unique.
  }
  /** \brief gets the counter value.
   *  \return number of alerts counted.
   */
  size_t get(void) const
  {
    return set_.size();                 // return number of unique entries.
  }

private:
  std::set<const void*> set_;
}; // struct TreeFuncObj
} // unnamed namespace


size_t countCorrelatedAlerts(Persistency::ConstGraphNodePtrNN root)
{
  return forEachInTree(root, AlertsCounter() ).get();
} // forEach()

} // namespace Algo
