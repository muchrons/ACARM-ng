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

//
// TODO: get list from http://www.dshield.org/ipsascii.html?limit=10000
//
// TODO: with curl++:
//
//       #include <curlpp/curlpp.hpp>
//       #include <curlpp/Easy.hpp>
//       #include <curlpp/Options.hpp>
//
//       curlpp::Cleanup   myCleanup;
//       std::stringstream ss;
//       ss << curlpp::options::Url("http://example.com");
//       catch: curlpp::LogicError and curlpp::RuntimeError
//

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
