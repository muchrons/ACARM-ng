/*
 * Sources.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Core/Sources.hpp"

using namespace Persistency;

namespace Core
{

Sources::Sources(void):
  log_("core.sources"),
  conn_( IO::create() ),
  inputs_( Input::create(queue_) )
{
  LOGMSG_INFO(log_, "created");
}

Sources::~Sources(void)
{
  LOGMSG_INFO(log_, "deallocating");
}

Persistency::GraphNodePtrNN Sources::read(void)
{
  LOGMSG_DEBUG(log_, "awaiting new alert");
  AlertPtrNN      alert=queue_.pop();
  LOGMSG_DEBUG(log_, "got new alert");

  IO::Transaction t( conn_->createNewTransaction("core_save_graphnode") );
  LOGMSG_DEBUG(log_, "new transaction opened");
  GraphNodePtrNN  leaf( new GraphNode(alert, conn_, t) );
  LOGMSG_DEBUG(log_, "creating object done - commiting transaction");

  t.commit();
  LOGMSG_INFO(log_, "alert and meta-alert successfuly written to data base");
  return leaf;
}

} // namespace Core
