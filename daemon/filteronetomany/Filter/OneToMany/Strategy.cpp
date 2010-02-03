/*
 * Strategy.cpp
 *
 */
#include "Filter/OneToMany/Strategy.hpp"

namespace Filter
{
namespace OneToMany
{

Strategy::Strategy(void):
  Filter::Strategy<detail::Data>("onetomany")
{
}

void Strategy::processImpl(Node               /*n*/,
                           NodesTimeoutQueue &/*ntq*/,
                           BackendProxy      &/*bp*/)
{
  // TODO
}

} // namespace OneToMany
} // namespace Filter
