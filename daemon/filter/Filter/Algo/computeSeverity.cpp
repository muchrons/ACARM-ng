/*
 * computeSeverity.cpp
 *
 */
#include <set>

#include "Filter/Algo/computeSeverity.hpp"
#include "Filter/Algo/forEachUniqueInTree.hpp"

namespace Filter
{
namespace Algo
{

namespace
{
/** \brief helper object computing severity.
 */
class SeverComput
{
public:
  /** \brief initialize object.
   */
  SeverComput(void):
    delta_(0),
    leafsCount_(0),
    leafsSeveritySum_(0)
  {
  }
  /** \brief work procedure itself.
   *  \param node root node to start computation from.
   */
  void operator()(Persistency::GraphNodePtrNN node)
  {
    if( node->isLeaf() )
    {
      ++leafsCount_;
      leafsSeveritySum_+=node->getAlert()->getSeverity().getLevel().toInt();
    }
    else // node
    {
      delta_+=node->getMetaAlert()->getSeverityDelta();
    }
  }
  /** \brief gets the computed value.
   *  \return total (sub)tree severity.
   */
  double get(void) const
  {
    // arithmetic average of all severities modified by delta.
    return (0.0+leafsSeveritySum_)/leafsCount_ + delta_;
  }

private:
  double delta_;
  int    leafsCount_;
  int    leafsSeveritySum_;
}; // struct SeverComput
} // unnamed namespace


double computeSeverity(Persistency::GraphNodePtrNN root)
{
  return forEachUniqueInTree(root, SeverComput() ).get();
} // computeSeverity()

} // namespace Algo
} // namespace Filter
