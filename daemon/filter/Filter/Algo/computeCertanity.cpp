/*
 * computeCertanity.cpp
 *
 */
#include <set>

#include "Filter/Algo/computeCertanity.hpp"
#include "Filter/Algo/forEachUniqueInTree.hpp"

namespace Filter
{
namespace Algo
{

namespace
{
/** \brief helper object computing certanity.
 */
class CertComput
{
public:
  /** \brief initialize object.
   */
  CertComput(void):
    delta_(0),
    leafsCount_(0),
    leafstySum_(0)
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
   *  \return total (sub)tree certanity.
   */
  double get(void) const
  {
    // arithmetic average of all severities modified by delta.
    return (0.0+leafsSeveritySum_)/leafsCount_ + delta_;
  }

private:
  double delta_;
  int    leafsCount_;
  int    leafsCertanitySum_;
}; // struct CertComput
} // unnamed namespace


double computeCertanity(Persistency::GraphNodePtrNN root)
{
  return forEachUniqueInTree(root, CertComput() ).get();
} // computeSeverity()

} // namespace Algo
} // namespace Filter
