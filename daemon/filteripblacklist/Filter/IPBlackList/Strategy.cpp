/*
 * Strategy.cpp
 *
 */
#include <vector>
#include <algorithm>
#include <cassert>

#include "Filter/IPBlackList/Strategy.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/IPBlackList/EntryProcessor.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace IPBlackList
{

Strategy::Strategy(const Parameters &params):
  Filter::Strategy<Data>("ipblacklist"),
  params_(params)
{
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendProxy      &bp)
{
  Algo::forEachUniqueLeaf(n, EntryProcessor(/*&cache_, */&bp) );
}

} // namespace IPBlackList
} // namespace Filter
