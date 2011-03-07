/*
 * EntryProcessor.cpp
 *
 */

#include "Filter/NewEvent/EntryProcessor.hpp"

namespace Filter
{
namespace NewEvent
{

EntryProcessor::EntryProcessor(BackendFacade             *bf,
                               ProcessedSet              *ps,
                               TimeoutedSet              *ts,
                               const Strategy::Parameters params):
 log_("filter.newevent"),
 bf_(bf),
 ps_(ps),
 ts_(ts),
 params_(params)
{
  assert(bf_!=NULL);
  assert(ps_!=NULL);
  // TODO: ts_==NULL?
}

void EntryProcessor::operator()(Persistency::GraphNodePtrNN leaf)
{
  assert( leaf->isLeaf() && "wrong graph-passing algorithm choosen" );
  const std::string name(leaf->getMetaAlert()->getName().get());
  // create helper object
  Entry e(name, bf_, ts_);
  assert(ps_!=NULL);

  if(ps_->isProcessed(e))
  {
    LOGMSG_DEBUG_S(log_)<<"(meta-)alert with name '"<<name<<"' has been already processed - skipping";
    return;
  }

  // TODO: old comment c&p from filteripblacklist
  // new host from black list - increase priority of the (meta-)alert
  assert(bf_!=NULL);
  LOGMSG_INFO_S(log_)<<"(meta-)alert with name "<< name
                     <<" is new - adding "<<params_.priDelta_<<" to priority";
  bf_->updateSeverityDelta(leaf, params_.priDelta_);
  ps_->markAsProcessed(e, params_.timeout_);     // mark change in cache to avoid doing it again.
}
} // namespace NewEvent
} // namespace Filter
