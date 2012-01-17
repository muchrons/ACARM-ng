/*
 * EntryProcessor.cpp
 *
 */

#include "Filter/NewEvent/EntryProcessor.hpp"

namespace Filter
{
namespace NewEvent
{

EntryProcessor::EntryProcessor(BackendFacade              &bf,
                               ProcessedSet               &ps,
                               TimeoutedSet               &ts,
                               const Strategy::Parameters &params):
 log_("filter.newevent"),
 bf_(bf),
 ps_(ps),
 ts_(ts),
 params_(params)
{
}

void EntryProcessor::operator()(Persistency::GraphNodePtrNN leaf)
{
  const std::string name(leaf->getMetaAlert()->getName().get());
  // check if given entry is leaf
  if(!leaf->isLeaf())
  {
    LOGMSG_DEBUG_S(log_)<<"(meta-)alert with name '"<<name<<"' is not leaf - skipping";
    return;
  }
  // create helper object
  const Hash hash(name);
  // check if entry with name hash is present in the prosessed set,
  // if is present update entry timeout and return
  if( ps_.update(hash, params_.timeout_) )
  {
    LOGMSG_DEBUG_S(log_)<<"(meta-)alert with name '"<<name<<"' has been already processed - skipping";
    return;
  }

  EntrySharedPtr e(new Entry(hash, bf_, ts_));
  // new (meta-)alert name - increase priority of the (meta-)alert
  LOGMSG_INFO_S(log_)<<"(meta-)alert with name "<< name
                     <<" is new - adding "<<params_.priDelta_<<" to priority";
  bf_.updateSeverityDelta(leaf, params_.priDelta_);
  ps_.markAsProcessed(e, params_.timeout_);     // mark change in cache to avoid doing it again.
}
} // namespace NewEvent
} // namespace Filter
