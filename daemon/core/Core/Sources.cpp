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
  AlertPtrNN      alert=queue_.pop();
  IO::Transaction t( conn_->createNewTransaction("core_save_graphnode") );
  GraphNodePtrNN  leaf( new GraphNode(alert, conn_, t) );
  return leaf;
}

} // namespace Core
