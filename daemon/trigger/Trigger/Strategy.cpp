/*
 * Strategy.cpp
 *
 */
#include <algorithm>
#include <boost/thread.hpp>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Trigger/Strategy.hpp"
#include "Trigger/BackendFacade.hpp"

using namespace std;


namespace
{
struct FindGivenNode
{
  explicit FindGivenNode(Persistency::GraphNodePtrNN n):
    n_(n)
  {
  }

  bool operator()(const boost::weak_ptr<Persistency::GraphNode> &w) const
  {
    return n_.get()==w.lock().get();
  }

private:
  Persistency::GraphNodePtrNN n_;
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

  // check if node should be processed at all
  if( !matchesCriteria(n) )
  {
    LOGMSG_DEBUG_S(log_)<<"node "<<n->getMetaAlert()->getID().get()<<" does not match criteria";
    return;
  }

  // process node
  LOGMSG_INFO_S(log_)<<"calling trigger for node "<<n->getMetaAlert()->getID().get();
  trigger(n);

  // if it succeeded, mark it as triggered
  nos_.add( n.shared_ptr() );
  // and save it to persistency storage
  BackendFacade bf(conn_, type_);
  bf.markAsTriggered( n->getMetaAlert() );
  bf.commitChanges();

  LOGMSG_INFO_S(log_)<<"triggering node "<<n->getMetaAlert()->getID().get()<<" finished successfully";
}


namespace
{
inline Logger::NodeName makeNodeName(const string &type, const string &name)
{
  const string &out="trigger." + Logger::NodeName::removeInvalidChars(type)
                               + "." + Logger::NodeName::removeInvalidChars(name);
  return Logger::NodeName( out.c_str() );
} // makeNodeName()
} // unnamed namespace

Strategy::Strategy(const std::string &type, const std::string &name):
  log_( makeNodeName(type, name) ),
  type_(type),
  conn_( Persistency::IO::create() )
{
  LOGMSG_INFO(log_, "trigger created");
}

void Strategy::interruptionPoint(void)
{
  boost::this_thread::interruption_point();
}

} // namespace Filter
