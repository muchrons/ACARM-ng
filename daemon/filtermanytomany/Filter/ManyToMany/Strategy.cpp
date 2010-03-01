/*
 * Strategy.cpp
 *
 */
#include "Filter/ManyToMany/Strategy.hpp"
#include <cassert>
#include "Logger/Logger.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Filter/ManyToMany/CheckHosts.hpp"

using namespace Persistency;

namespace Filter
{
namespace ManyToMany
{

Strategy::Strategy(void):
  Filter::Simple::Strategy<Data>("manytomany", 5*60)
{
}

Strategy::NodeEntry Strategy::makeThisEntry(const Node n) const
{
  // TODO
}

bool Strategy::isEntryInteresting(const NodeEntry &thisEntry) const
{
  // TODO
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const
{
  // TODO
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // TODO
}

} // namespace ManyToMany
} // namespace Filter
