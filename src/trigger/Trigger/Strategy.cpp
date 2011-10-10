/*
 * Strategy.cpp
 *
 */
#include <sstream>
#include <algorithm>
#include <boost/thread.hpp>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Trigger/Strategy.hpp"

using namespace std;


namespace
{
struct FindGivenNode
{
  explicit FindGivenNode(Persistency::ConstGraphNodePtrNN n):
    n_(n)
  {
  }

  bool operator()(const boost::weak_ptr<const Persistency::GraphNode> &w) const
  {
    return n_.get()==w.lock().get();
  }

private:
  Persistency::ConstGraphNodePtrNN n_;
}; // struct FindGivenNode
} // unnamed namespace


namespace Trigger
{

Strategy::~Strategy(void)
{
  LOGMSG_INFO(log_, "deallocating trigger");
}


void Strategy::process(Node n, ChangedNodes &/*changed*/)
{
  LOGMSG_DEBUG_S(log_)<<"processing node "<<n->getMetaAlert()->getID().get();
  // clean-up old entries
  nos_.prune();

  // check nos_ cache - maybe entry has been already triggered before?
  if( find_if( nos_.begin(), nos_.end(), FindGivenNode(n) )!=nos_.end() )
  {
    LOGMSG_DEBUG_S(log_)<<"node "<<n->getMetaAlert()->getID().get()<<" matches criteria but was already triggered before";
    return;
  }

  BackendFacade bf(conn_, type_, name_);

  // check if node should be processed at all
  if( !matchesCriteria(bf, n) )
  {
    LOGMSG_DEBUG_S(log_)<<"node "<<n->getMetaAlert()->getID().get()<<" does not match criteria";
    bf.commitChanges();
    return;
  }

  // process node
  LOGMSG_INFO_S(log_)<<"calling trigger for node "<<n->getMetaAlert()->getID().get();
  trigger(bf, n);
  bf.commitChanges();

  // if it succeeded, mark it as triggered
  nos_.add( n.shared_ptr() );
  // and save it to persistency storage
  bf.markAsTriggered( n->getMetaAlert() );
  bf.commitChanges();

  LOGMSG_INFO_S(log_)<<"triggering node "<<n->getMetaAlert()->getID().get()<<" finished successfully";
}


void Strategy::heartbeat(const unsigned int deadline)
{
  // notice that this is Trigger::BackednFacade, and so why this code is valid.
  BackendFacade bf(conn_, type_, name_);
  bf.heartbeat(deadline);
  bf.commitChanges();
}


namespace
{
inline Logger::NodeName makeNodeName(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name)
{
  const string &out="trigger." + Logger::NodeName::removeInvalidChars(type.str())
                               + "." + Logger::NodeName::removeInvalidChars(name.str());
  return Logger::NodeName( out.c_str() );
} // makeNodeName()
} // unnamed namespace

Strategy::Strategy(const Core::Types::Proc::TypeName &type, const Core::Types::Proc::InstanceName &name):
  log_( makeNodeName(type, name) ),
  type_(type),
  name_(name),
  conn_( Persistency::IO::create() )
{
  LOGMSG_INFO(log_, "trigger created");
}

void Strategy::interruptionPoint(void)
{
  boost::this_thread::interruption_point();
}

} // namespace Filter
