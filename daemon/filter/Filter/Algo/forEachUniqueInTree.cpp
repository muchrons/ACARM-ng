/*
 * forEachUniqueInTree.hpp
 *
 */
#include <cassert>

#include "Filter/Algo/forEachUniqueInTree.hpp"
#include "Filter/Algo/forEachInTree.hpp"

namespace Filter
{
namespace Algo
{
namespace detail
{

namespace
{
class SaveUnique
{
public:
  explicit SaveUnique(NodesSet *ns):
    ns_(ns)
  {
    assert(ns_!=NULL);
  }
  void operator()(Persistency::GraphNodePtrNN node)
  {
    assert(ns_!=NULL);
    ns_->insert(node);
  }

private:
  NodesSet *ns_;
}; // struct TreeFuncObj
} // unnamed namespace


void findUniqueNodes(Persistency::GraphNodePtrNN root, NodesSet &ns)
{
  assert( ns.size()==0 );
  forEachInTree(root, SaveUnique(&ns) );
} // findUniqueNodes()

} // namespace detail
} // namespace Algo
} // namespace Filter
