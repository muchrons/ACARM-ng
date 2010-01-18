/*
 * forEachUniqueInTree.hpp
 *
 */
#include <cassert>

#include "Algo/forEachUniqueInTree.hpp"
#include "Algo/forEachInTree.hpp"

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
