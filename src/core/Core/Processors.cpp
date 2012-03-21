/*
 * Processors.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Persistency/GraphNode.hpp"
#include "Filter/Factory.hpp"
#include "Trigger/Factory.hpp"
#include "Core/Processors.hpp"
#include "Commons/setThreadName.hpp"

using namespace Persistency;
using namespace Core::Types::Proc;

namespace Core
{

Processors::Processors(Core::Types::SignedNodesFifo &queue):
  log_("core.processors"),
  queue_(queue)
{
  LOGMSG_INFO(log_, "building processors");
  append( Filter::create(queue) );  // create filters
  append( Trigger::create(queue) ); // create triggers
  Commons::setThreadName("processors");
  LOGMSG_INFO_S(log_)<<"total "<<procs_.size()<<" processors created";
}

Processors::~Processors(void)
{
  LOGMSG_INFO(log_, "deallocating all processors");
}

void Processors::process(void)
{
  LOGMSG_DEBUG_S(log_)<<"waiting for nodes to process (current main queue's size is: "
                      <<queue_.size()<<" element(s))";
  Core::Types::SignedNode node=queue_.pop(); // wait for next element
  LOGMSG_DEBUG(log_, "got node to process");

  // process it with all the processors
  for(ProcessorsCollection::iterator it=procs_.begin(); it!=procs_.end(); ++it)
    (*it)->process(node);
}

void Processors::append(const Core::Types::Proc::ProcessorsCollection &newProcs)
{
  for(ProcessorsCollection::const_iterator it=newProcs.begin(); it!=newProcs.end(); ++it)
    procs_.push_back(*it);
}

} // namespace Core
