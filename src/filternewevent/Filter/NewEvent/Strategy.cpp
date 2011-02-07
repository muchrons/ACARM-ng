/*
 * Strategy.cpp
 *
 */

#include "Filter/NewEvent/Strategy.hpp"

namespace Filter
{
namespace NewEvent
{

Strategy::Strategy(const std::string &name, const Parameters &params):
  Filter::Strategy<Data>("newevent", name),
  params_(params)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
    Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
      ecl.add("*input*");   // TODO: magic value
        return ecl;
}

void Strategy::processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendFacade     &bf)
{
  //TODO
}

} // namespace NewEvent
} // namespace Filter
