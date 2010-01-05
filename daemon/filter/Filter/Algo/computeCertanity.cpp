/*
 * computeCertanity.cpp
 *
 */
#include <set>
#include <cassert>

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
    leafsCertanitySum_(0)
  {
  }
  /** \brief work procedure itself.
   *  \param node root node to start computation from.
   */
  void operator()(Persistency::GraphNodePtrNN node)
  {
std::cerr<<"! "<<leafsCertanitySum_<<" / "<<leafsCount_<<" / "<<delta_<<"\n";                       
    if( node->isLeaf() )
    {
      assert( node->getMetaAlert()->getCertanityDelta()==0 &&
              "non-zero certaity delta for leaf detected"     );
      ++leafsCount_;
      leafsCertanitySum_+=node->getAlert()->getCertanity().get();
    }
    else // node
    {
      delta_+=node->getMetaAlert()->getCertanityDelta();
    }
std::cerr<<"> "<<leafsCertanitySum_<<" / "<<leafsCount_<<" / "<<delta_<<"\n";                       
  }
  /** \brief gets the computed value.
   *  \return total (sub)tree certanity.
   */
  double get(void) const
  {
    // arithmetic average of all certanities modified by delta.
    return (0.0+leafsCertanitySum_)/leafsCount_ + delta_;
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
