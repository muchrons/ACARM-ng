/*
 * computeCertainty.cpp
 *
 */
#include <set>
#include <cassert>

#include "Algo/computeCertainty.hpp"
#include "Algo/forEachUniqueInTree.hpp"

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
    leafsCertaintySum_(0)
  {
  }
  /** \brief work procedure itself.
   *  \param node root node to start computation from.
   */
  void operator()(Persistency::ConstGraphNodePtrNN node)
  {
    if( node->isLeaf() )
    {
      assert( node->getMetaAlert()->getCertaintyDelta()==0 &&
              "non-zero certaity delta for leaf detected"     );
      ++leafsCount_;
      leafsCertaintySum_+=node->getAlert()->getCertainty().get();
    }
    else // node
    {
      delta_+=node->getMetaAlert()->getCertaintyDelta();
    }
  }
  /** \brief gets the computed value.
   *  \return total (sub)tree certanity.
   */
  double get(void) const
  {
    // arithmetic average of all certanities modified by delta.
    return (0.0+leafsCertaintySum_)/leafsCount_ + delta_;
  }

private:
  double delta_;
  int    leafsCount_;
  double leafsCertaintySum_;
}; // struct CertComput
} // unnamed namespace


double computeCertainty(Persistency::ConstGraphNodePtrNN root)
{
  return forEachUniqueInTree(root, CertComput() ).get();
} // computeSeverity()

} // namespace Algo
