/*
 * Sources.cpp
 *
 */
#include "ConfigIO/Singleton.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "DataFacades/IDAssigner.hpp"
#include "Core/Sources.hpp"

using namespace Persistency;

namespace Core
{

Sources::Sources(void):
  log_("core.sources"),
  conn_( IO::create() ),
  inputs_( Input::create(queue_) ),
  preproc_( ConfigIO::Singleton::get()->preprocessorConfig() )
{
  LOGMSG_INFO(log_, "created");
}

Sources::~Sources(void)
{
  LOGMSG_INFO(log_, "deallocating");
}

Persistency::GraphNodePtrNN Sources::read(void)
{
  // loop here ensures that reading will not stop when alert has been read
  // but rejected by the preprocessor.

  while(true)
  {
    // read alert from input(s)
    LOGMSG_DEBUG(log_, "awaiting new alert");
    const AlertPtrNN alert=queue_.pop();
    LOGMSG_DEBUG(log_, "got new alert");

    // write it to data base along with creating proper graph-node object.
    IO::Transaction      t( conn_->createNewTransaction("core_save_graphnode") );
    LOGMSG_DEBUG(log_, "new transaction opened");
    const GraphNode::ID  id=DataFacades::IDAssigner::get()->assignMetaAlertID(conn_, t);
    const GraphNodePtrNN leaf( new GraphNode(alert, id, conn_, t) );
    LOGMSG_DEBUG(log_, "creating object done (delaying commit of transactionuntil preprocessor's done)");
    LOGMSG_INFO_S(log_)<<"alert and meta-alert successfuly written to persistency (ID="<<id.get()<<")";

    // check if the pre-processor accepts given alert
    LOGMSG_DEBUG(log_, "checking if alert is accepted by the preprocessor");
    if( preproc_.checkAccept(leaf) )
    {
      LOGMSG_DEBUG_S(log_)<<"alert "<<id.get()<<" accepted by the preprocessor - commiting";
      t.commit();

      //wait if we are short of memory
      if (memCheck_.iSmemoryLimitExceeded())
        sleep(1);
      return leaf;
    }
    LOGMSG_INFO_S(log_)<<"alert '"<<alert->getName().get()<<"' rejected by the preprocessor";
    LOGMSG_DEBUG(log_, "rolling back transaction that saves rejected meta-alert");
    t.rollback();
  } // while(true)
}

} // namespace Core
