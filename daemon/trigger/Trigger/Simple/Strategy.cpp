/*
 * Strategy.cpp
 *
 */
#include <iostream>

#include "Algo/computeSeverity.hpp"
#include "Algo/countCorrelatedAlerts.hpp"
#include "Trigger/Simple/Strategy.hpp"

using namespace std;

namespace Trigger
{
namespace Simple
{

Strategy::Strategy(const std::string &name, const ThresholdConfig &cfg):
  Trigger::Strategy(name),
  cfg_(cfg)
{
}

Strategy::~Strategy(void)
{
  const size_t cnt=fifo_.size();
  if(cnt>0)
    LOGMSG_WARN_S(log_)<<"trigger killed while having "<<cnt
                       <<" alerts in sending queue - discarding them permanently";
}

bool Strategy::matchesCriteria(const Persistency::GraphNodePtrNN &n) const
{
  // check severity
  if( cfg_.getSeverityThreshold().get()!=NULL )
    if( *cfg_.getSeverityThreshold().get() <= Algo::computeSeverity(n) )
      return true;

  // check alerts count
  if( cfg_.getAlertCountThreshold().get()!=NULL )
    if( *cfg_.getAlertCountThreshold().get() <= Algo::countCorrelatedAlerts(n) )
      return true;

  // if no criteria matches, report no match.
  return false;
}

void Strategy::trigger(const Node &n)
{
  // add new element to queue
  if( fifo_.maxSize()==fifo_.size() )
    LOGMSG_WARN(log_, "queue has reached maximum size - the oldest message is being lost...");
  fifo_.push(n);

  // send all enqueued messages
  LOGMSG_DEBUG_S(log_)<<"got total of "<<fifo_.size()<<" reports to be send";
  do
  {
    LOGMSG_DEBUG(log_, "sending next message...");
    // sending itself
    triggerImpl( fifo_.top() );     // process
    fifo_.pop();                    // if no problems were reported, remove it from queue
    LOGMSG_DEBUG(log_, "message send successfuly - removed from queue");
  }
  while( fifo_.size()>0 );
  // done!
  LOGMSG_DEBUG(log_, "all messages successfuly sent - queue's empty");
}

} // namespace Simple
} // namespace Filter
